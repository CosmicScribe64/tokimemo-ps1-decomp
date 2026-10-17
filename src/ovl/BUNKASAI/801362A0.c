#include "common.h"
#include "ovl/BUNKASAI.h"

void func_801362A0(void) {
    BunkaDispatch8 *dispatch = (BunkaDispatch8 *)&D_80122EB8;
    dispatch->unk_04 = 4;
    switch (dispatch->unk_00) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x57);
            return;
        }
        func_80042908(0x56);
        return;
    case 1:
        func_80042908(0x58);
        return;
    case 2:
        func_80042908(0x59);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x5A);
            return;
        case 0x60:
            func_80042908(0x5C);
            return;
        case 0x61:
            func_80042908(0x59);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x5D);
            return;
        case 0x60:
            func_80042908(0x5E);
            return;
        case 0x61:
            func_80042908(0x5F);
            return;
        }
        break;
    case 5:
        func_80042908(0x60);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80136414(void) {
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
            func_80062CD0(0x66BF);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 4;
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
        cd_read_sectors(0xAU, 0x801E8000, 0x4B47);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 5;
                break;
            case 2:
            case 3:
                D_800CA148 = 6;
                break;
            case 4:
                D_800CA148 = 7;
                break;
            }
            if (D_800E6280.unk_03E == 0x60) {
                D_800CA148 += 3;
            }
            func_80138A00();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(4) == 4) {
            func_80042940(0xC);
        } else {
            temp_v0_2 = func_8008667C(4);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 1;
                break;
            case 2:
            case 3:
                D_800CA148 = 2;
                break;
            }
            func_8004284C();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        D_800CA148 = 0x56;
        normal_date_girl_out();
        break;
    case 13:
        if (func_8008667C(4) == 4) {
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
        if (func_8008667C(4) != 4) {
            if (D_800E6280.unk_03E == 0x5F) {
                D_800E6280.unk_1BC[4].unk_0C.f.b12 = 1;
            } else {
                D_800E6280.unk_1BC[4].unk_0C.f.b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_801367A0(void) {
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
        func_800866A0(0x66BF);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 4;
        func_80083474();
        cd_read_sectors(0xAU, 0x801E8000, 0x4B47);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0xB;
                break;
            case 2:
            case 3:
                D_800CA148 = 0xC;
                break;
            case 4:
                D_800CA148 = 0xD;
                break;
            }
            func_80138A00();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(4);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
        case 3:
            D_800CA148 = 2;
            break;
        case 4:
            D_800CA148 = 3;
            break;
        }
        func_8004284C();
        break;
    case 14:
        if (func_8008667C(4) == 4) {
            normal_date_girl_out();
        } else {
            func_8004284C();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (func_8008667C(4) == 4) {
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
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_80136AE8(void) {
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
            func_800866A0(0x66BF);
        }
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8014338C();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            D_800E6280.unk_F5F = 4;
            func_80083474();
            cd_read_sectors(0xAU, 0x801E8000, 0x4B47);
        }
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0xE;
                break;
            case 2:
            case 3:
                D_800CA148 = 0xF;
                break;
            case 4:
                D_800CA148 = 0x10;
                break;
            }
            D_800CA148 = (D_800CA148 + (D_800E6280.unk_03E * 3)) - 0x11D;
            func_80138A00();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(4);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
        case 3:
            D_800CA148 = 2;
            break;
        case 4:
            D_800CA148 = 3;
            break;
        }
        func_8004284C();
        break;
    case 14:
        if (func_8008667C(4) == 4) {
            normal_date_girl_out();
        } else {
            func_8004284C();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (func_8008667C(4) == 4) {
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
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_80136EC8(void) {
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
            func_80062CD0(0x66BF);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 4;
            func_80083474();
            D_800CA148 = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xAU, 0x801E8000, 0x4B47);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x17;
                break;
            case 2:
            case 3:
                D_800CA148 = 0x18;
                break;
            case 4:
                D_800CA148 = 0x19;
                break;
            }
            func_80138A00();
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
        func_800866A0(0x66BF);
        break;
    case 13:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 4;
        func_80083474();
        cd_read_sectors(0xAU, 0x801E8000, 0x4B47);
        break;
    case 14:
        if (func_8008667C(4) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(4);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = (D_800E6280.unk_03E * 2) - 0xA4;
                break;
            case 2:
            case 3:
                D_800CA148 = (D_800E6280.unk_03E * 2) - 0xA3;
                break;
            case 4:
                D_800CA148 = 4;
                break;
            }
            func_80138A00();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_8004284C();
        temp_v0_3 = func_8008667C(4);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
        case 3:
            D_800CA148 = 2;
            break;
        case 4:
            func_80042940(0x14);
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
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_80137304(void) {
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
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x66BF);
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
            D_800E6280.unk_F5F = 4;
            func_80083474();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xAU, 0x801E8000, 0x4B47);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x20;
                break;
            case 2:
            case 3:
                D_800CA148 = 0x21;
                break;
            case 4:
                D_800CA148 = 0x22;
                break;
            }
            func_80138A00();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        if (func_8008667C(4) == 4) {
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
        cd_read_sectors(0xAU, 0x801E8000, 0x4B47);
        func_8004284C();
        break;
    case 14:
        if (func_8008667C(4) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(4);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x24;
                break;
            case 2:
            case 3:
                D_800CA148 = 0x27;
                break;
            case 4:
                D_800CA148 = 0x2A;
                break;
            }
            D_800CA148 += D_80122EA8;
            func_80138A00();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_8004284C();
        temp_v0_3 = func_8008667C(4);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x2D;
            break;
        case 2:
        case 3:
            D_800CA148 = 0x2E;
            break;
        case 4:
            func_80042940(0x16);
            break;
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        temp_v0_4 = func_8008667C(4);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
        case 3:
            D_800CA148 = 2;
            break;
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
        normal_date_bg_fadeout();
        break;
    case 23:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_80137788(void) {
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
        func_800866A0(0x66BF);
        break;
    case 4:
        addr_init_bustup();
        D_800E6280.unk_F5F = 4;
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
        cd_read_sectors(0xAU, 0x801E8000, 0x4B47);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x2F;
                break;
            case 2:
            case 3:
                D_800CA148 = 0x33;
                break;
            case 4:
                D_800CA148 = 0x37;
                break;
            }
            func_80138A00();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (func_8008667C(4) == 4) {
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
        if (func_8008667C(4) == 4) {
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
        func_800847B8(4U);
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
        func_800866A0(0x66BF);
        break;
    case 21:
        addr_init_bustup();
        D_800E6280.unk_F5F = 4;
        func_80083474();
        cd_read_sectors(0xAU, 0x801E8000, 0x4B47);
        break;
    case 22:
        if (func_8008667C(4) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(4);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x30;
                break;
            case 2:
            case 3:
                D_800CA148 = 0x34;
                break;
            case 4:
                D_800CA148 = 0x39;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                D_800CA148 += 1;
            }
            func_80138A00();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        func_8004284C();
        temp_v0_3 = func_8008667C(4);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x32;
            break;
        case 2:
        case 3:
            D_800CA148 = 0x36;
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
        temp_v0_4 = func_8008667C(4);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
        case 3:
            D_800CA148 = 2;
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


void func_80137DD8(void) {
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
            func_80062CD0(0x66BF);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 4;
            func_80083474();
            D_800CA148 = 0x2B;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xAU, 0x801E8000, 0x4B47);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x3B;
                break;
            case 2:
            case 3:
                D_800CA148 = 0x3F;
                break;
            case 4:
                D_800CA148 = 0x43;
                break;
            }
            func_80138A00();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(4) == 4) {
            func_80042940(0x10);
        } else {
            if ((u16) D_800E6280.unk_66C[4].unk_00 < 0x64U) {
                D_800CA148 += 1;
            }
            func_8004284C();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = func_8008667C(4);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x3E;
            break;
        case 2:
        case 3:
            D_800CA148 = 0x42;
            break;
        }
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        temp_v0_3 = func_8008667C(4);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
        case 3:
            D_800CA148 = 2;
            break;
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


void func_80138148(void) {
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
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x66BF);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 4;
            func_80083474();
            D_800CA148 = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xAU, 0x801E8000, 0x4B47);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x44;
                break;
            case 2:
            case 3:
                D_800CA148 = 0x46;
                break;
            case 4:
                D_800CA148 = 0x48;
                break;
            }
            func_80138A00();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_bg_fadeout();
        break;
    case 13:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80138388(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            func_80062CD0(0x66BF);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 4;
            func_80083474();
            D_800CA148 = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xAU, 0x801E8000, 0x4B47);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x4A;
                break;
            case 2:
            case 3:
                D_800CA148 = 0x4B;
                break;
            case 4:
                D_800CA148 = 0x4C;
                break;
            }
            func_80138A00();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        func_8004284C();
        temp_v0_2 = func_8008667C(4);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
        case 3:
            D_800CA148 = 2;
            break;
        case 4:
            D_800CA148 = 0x57;
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
        if (func_8008667C(4) == 4) {
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


void func_8013866C(void) {
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
        if (D_800E6280.unk_03E == 0x5F) {
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
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x66BF);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 4;
        func_80083474();
        cd_read_sectors(0xAU, 0x801E8000, 0x4B47);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x4D;
                break;
            case 2:
            case 3:
                D_800CA148 = 0x4E;
                break;
            case 4:
                D_800CA148 = 0x4F;
                break;
            }
            D_800CA148 = (D_800CA148 + (D_800E6280.unk_03E * 3)) - 0x11D;
            func_80138A00();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(4);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
        case 3:
            D_800CA148 = 2;
            break;
        case 4:
            D_800CA148 = 3;
            break;
        }
        func_8004284C();
        break;
    case 14:
        if (func_8008667C(4) == 4) {
            normal_date_girl_out();
        } else {
            func_8004284C();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (func_8008667C(4) == 4) {
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
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_80138A00(void) {
    func_800847B8(4);
    func_8013223C();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
