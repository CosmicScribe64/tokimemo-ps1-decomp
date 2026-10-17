#include "common.h"
#include "ovl/BUNKASAI.h"

void func_8014ABC0(void) {
    BunkaDispatch8 *dispatch = (BunkaDispatch8 *)&D_80122EB8;
    dispatch->unk_04 = 5;
    switch (dispatch->unk_00) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x63);
            return;
        }
        func_80042908(0x62);
        return;
    case 1:
        func_80042908(0x64);
        return;
    case 2:
        func_80042908(0x65);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x66);
            return;
        case 0x60:
            func_80042908(0x68);
            return;
        case 0x61:
            func_80042908(0x65);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x69);
            return;
        case 0x60:
            func_80042908(0x6A);
            return;
        case 0x61:
            func_80042908(0x6B);
            return;
        }
        break;
    case 5:
        func_80042908(0x6C);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8014AD34(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

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
            func_80062CD0(0x5FB7);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 5;
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
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(5);
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
            func_8014DD28();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x10;
            break;
        case 2:
            D_800CA148 = 0x11;
            break;
        case 3:
            D_800CA148 = 0x12;
            break;
        case 4:
            D_800CA148 = 0x13;
            break;
        }
        if (D_800E6280.unk_03E == 0x60) {
            D_800CA148 += 4;
        }
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_3 = func_8008667C(5);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 8;
            break;
        case 2:
            D_800CA148 = 9;
            break;
        case 3:
            D_800CA148 = 0xA;
            break;
        case 4:
            D_800CA148 = 0x36;
            func_8004284C();
            break;
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
        if (func_8008667C(5) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 16:
        func_8004284C();
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
        if (func_8008667C(5) != 4) {
            if (D_800E6280.unk_03E == 0x5F) {
                D_800E6280.unk_1BC[5].unk_0C.f.b12 = 1;
            } else {
                D_800E6280.unk_1BC[5].unk_0C.f.b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014B15C(void) {
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
        func_800866A0(0x5FB7);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 5;
        func_80083474();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(5);
            switch (temp_v0) {
            case 0:
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
                D_800CA148 = 6;
                break;
            }
            func_8014DD28();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(5);
        switch (temp_v0_2) {
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
            D_800CA148 = 0xE;
            break;
        }
        func_8004284C();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if ((func_8008667C(5) == 4) || (func_8008667C(5) == 2)) {
            func_80042940(0x12);
        } else {
            func_8004284C();
        }
        break;
    case 16:
        if (func_8008667C(5) == 3) {
            D_800CA148 = 0xA;
        } else {
            D_800CA148 = 8;
        }
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        normal_date_girl_out();
        break;
    case 19:
        if (func_8008667C(5) == 4) {
            func_80042940(0x15);
        } else {
            D_800CA148 = 0x6F;
            func_8004284C();
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

void func_8014B548(void) {
    s32 temp_v0_3;
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
        if (D_800E6280.unk_03E == 0x5F) {
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
            func_800866A0(0x5FB7);
        }
        break;
    case 9:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 5;
        func_80083474();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(5);
            switch (temp_v0) {
            case 0:
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
                D_800CA148 = 6;
                break;
            }
            func_8014DD28();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = (D_800E6280.unk_03E * 3) - 0x102;
            break;
        case 2:
            D_800CA148 = (D_800E6280.unk_03E * 3) - 0x101;
            break;
        case 3:
            D_800CA148 = (D_800E6280.unk_03E * 3) - 0x100;
            break;
        case 4:
            D_800CA148 = 0xE;
            break;
        }
        func_8004284C();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (func_8008667C(5) == 4) {
            func_80042940(0x12);
        } else {
            func_8004284C();
        }
        break;
    case 16:
        temp_v0_3 = func_8008667C(5);
        switch (temp_v0_3) {                        /* switch 4; irregular */
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
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        normal_date_girl_out();
        break;
    case 19:
        if (func_8008667C(5) == 4) {
            func_80042940(0x15);
        } else {
            D_800CA148 = 0x6F;
            func_8004284C();
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

void func_8014B9D8(void) {
    s32 temp_v0_4;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    GameState *g = &D_800E6280;

    switch (g->unk_110A) {
    case 0:
        if (((u16) g->unk_0F4.h >> 0xC) == 2) {
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
            func_80062CD0(0x5FB7);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->unk_F5F = 5;
            func_80083474();
            D_800CA148 = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(5);
            switch (temp_v0) {
            case 0:
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
            func_8014DD28();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        D_800CA148 = 0x24;
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = func_8008667C(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x25;
            break;
        case 2:
            D_800CA148 = 0x26;
            break;
        case 3:
            D_800CA148 = 0x27;
            break;
        case 4:
            D_800CA148 = 0x28;
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
        D_80122EAC = 0;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 16:
        func_800866A0(0x5FB7);
        break;
    case 17:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->unk_F5F = 5;
        func_80083474();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 18:
        if (func_8008667C(5) == 4) {
            normal_date_bg_fadein();
        } else {
            func_8007E884();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(5);
            switch (temp_v0_3) {
            case 0:
            case 1:
                if (((u16) g->unk_0F4.h >> 0xC) == 3) {
                    D_800CA148 = 0x32;
                } else {
                    D_800CA148 = (g->unk_03E * 3) - 0xF4;
                }
                break;
            case 2:
                if (((u16) g->unk_0F4.h >> 0xC) == 3) {
                    D_800CA148 = 0x33;
                } else {
                    D_800CA148 = (g->unk_03E * 3) - 0xF3;
                }
                break;
            case 3:
                if (((u16) g->unk_0F4.h >> 0xC) == 3) {
                    D_800CA148 = 0x34;
                } else {
                    D_800CA148 = (g->unk_03E * 3) - 0xF2;
                }
                break;
            case 4:
                D_800CA148 = 0xF;
            }
            func_8014DD28();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (func_8008667C(5) == 4) {
            func_80042940(0x1A);
        } else {
            temp_v0_4 = func_8008667C(5);
            switch (temp_v0_4) {                    /* switch 5; irregular */
            case 0:
            case 1:
                D_800CA148 = 8;
                break;
            case 2:
                D_800CA148 = 9;
                break;
            case 3:
                D_800CA148 = 0xA;
                break;
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
        if (((u16) g->unk_0F4.h >> 0xC) == 3) {
            D_800CA148 = 0x35;
        } else {
            D_800CA148 = 0x6F;
        }
        func_8004284C();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        normal_date_bg_fadeout();
        break;
    case 27:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014BFE0(void) {
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
            func_80062CD0(0x5FB7);
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
            D_800E6280.unk_F5F = 5;
            func_80083474();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(5);
            switch (temp_v0) {
            case 0:
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
            func_8014DD28();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x37;
            break;
        case 2:
            D_800CA148 = 0x38;
            break;
        case 3:
            D_800CA148 = 0x39;
            break;
        case 4:
            D_800CA148 = 0x3A;
            break;
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
        if (func_8008667C(5) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 14:
        func_80044750(0x202);
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042808();
        break;
    case 15:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        func_8004284C();
        break;
    case 16:
        if (func_8008667C(5) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            func_8014DD28();
        }
        break;
    case 18:
        func_8004284C();
        break;
    case 19:
        D_800CA148 = D_80122EA8 + 0x3D;
        if (func_8008667C(5) == 4) {
            D_800CA148 += 3;
        }
        func_8004284C();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        func_8004284C();
        temp_v0_3 = func_8008667C(5);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x43;
            break;
        case 2:
            D_800CA148 = 0x44;
            break;
        case 3:
            D_800CA148 = 0x45;
            break;
        case 4:
            func_80042940(0x1C);
            break;
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        temp_v0_4 = func_8008667C(5);
        switch (temp_v0_4) {                        /* switch 5; irregular */
        case 0:
        case 1:
            D_800CA148 = 8;
            break;
        case 2:
            D_800CA148 = 9;
            break;
        case 3:
            D_800CA148 = 0xA;
            break;
        }
        func_8004284C();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        normal_date_girl_out();
        break;
    case 26:
        D_800CA148 = 0x6F;
        func_8004284C();
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        normal_date_bg_fadeout();
        break;
    case 29:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014C528(void) {
    s32 temp_v0_3;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_4;
    u32 temp_v0_5;

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
        func_800866A0(0x5FB7);
        break;
    case 4:
        addr_init_bustup();
        D_800E6280.unk_F5F = 5;
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
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(5);
            switch (temp_v0) {
            case 0:
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
            func_8014DD28();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        D_800CA148 = 0x46;
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x47;
            break;
        case 2:
            D_800CA148 = 0x48;
            break;
        case 3:
            D_800CA148 = 0x49;
            break;
        case 4:
            D_800CA148 = 0x4A;
            break;
        }
        func_8004284C();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (func_8008667C(5) == 4) {
            normal_date_girl_out();
        } else {
            func_80042940(0x12);
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80042940(0x1D);
        break;
    case 18:
        normal_date_bggirl_fadeout();
        break;
    case 19:
        ((Flags7D4F *)&D_800E7D4F)->b1 = 1;
        ((Flags7D4F *)&D_800E7D4F)->unk_2_4 = 1;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        main_state_set(0xD0);
        break;
    case 20:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xA3);
        break;
    case 21:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        func_80084E4C();
        func_800847B8(5U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 22:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            func_8004284C();
        }
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 24:
        func_800866A0(0x5FB7);
        break;
    case 25:
        addr_init_bustup();
        D_800E6280.unk_F5F = 5;
        func_80083474();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 26:
        func_8007E884();
        break;
    case 27:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(5);
            switch (temp_v0_3) {                    /* switch 4; irregular */
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
            }
            func_8014DD28();
        }
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        if (func_8008667C(5) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 30:
        ((Flags7D4F *)&D_800E7D4F)->b1 = 1;
        ((Flags7D4F *)&D_800E7D4F)->unk_2_4 = 0;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        func_80042908(0xA4);
        break;
    case 31:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xA3);
        break;
    case 32:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        func_80084E4C();
        func_800847B8(5U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 33:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            func_8004284C();
        }
        break;
    case 34:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 35:
        func_800866A0(0x5FB7);
        break;
    case 36:
        addr_init_bustup();
        D_800E6280.unk_F5F = 5;
        func_80083474();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 37:
        if (func_8008667C(5) == 4) {
            normal_date_bg_fadein();
        } else {
            func_8007E884();
        }
        break;
    case 38:
        if (cd_read_sync_status() & 1) {
            temp_v0_4 = func_8008667C(5);
            switch (temp_v0_4) {
            case 0:
            case 1:
                D_800CA148 = 0x52;
                break;
            case 2:
                D_800CA148 = 0x53;
                break;
            case 3:
                D_800CA148 = 0x54;
                break;
            case 4:
                D_800CA148 = 0x55;
                break;
            }
            func_8014DD28();
        }
        break;
    case 39:
        normal_date_speak();
        break;
    case 40:
        func_8004284C();
        temp_v0_5 = func_8008667C(5);
        switch (temp_v0_5) {
        case 0:
        case 1:
            D_800CA148 = 8;
            break;
        case 2:
            D_800CA148 = 9;
            break;
        case 3:
            D_800CA148 = 0xA;
            break;
        case 4:
            func_80042940(0x2B);
            break;
        }
        break;
    case 41:
        normal_date_speak();
        break;
    case 42:
        D_800CA148 = 0x6F;
        normal_date_girl_out();
        break;
    case 43:
        if (func_8008667C(5) == 4) {
            normal_date_bg_fadeout();
        } else {
            normal_date_speak();
        }
        break;
    case 44:
        D_800CA148 = 0;
        if (func_8008667C(5) == 4) {
            func_8004284C();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 45:
        normal_date_speak();
        break;
    case 46:
        ((Flags7D4F *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014CE94(void) {
    s32 temp_v0_3;
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
            func_80062CD0(0x5FB7);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 5;
            func_80083474();
            D_800CA148 = 0x2B;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(5);
            switch (temp_v0) {
            case 0:
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
            func_8014DD28();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        D_800CA148 = 0x57;
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = func_8008667C(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x58;
            break;
        case 2:
            D_800CA148 = 0x59;
            break;
        case 3:
            D_800CA148 = 0x5A;
            break;
        case 4:
            D_800CA148 = 0x5B;
            break;
        }
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if (func_8008667C(5) == 4) {
            func_80042940(0x10);
        } else {
            temp_v0_3 = func_8008667C(5);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                D_800CA148 = 8;
                break;
            case 2:
                D_800CA148 = 9;
                break;
            case 3:
                D_800CA148 = 0xA;
                break;
            }
            func_8004284C();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_girl_out();
        break;
    case 17:
        func_8004284C();
        break;
    case 18:
        func_8004284C();
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

void func_8014D240(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            func_80062CD0(0x5FB7);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 5;
            func_80083474();
            D_800CA148 = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(5);
            switch (temp_v0) {
            case 0:
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
            func_8014DD28();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        D_800CA148 = 0x5C;
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = func_8008667C(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x5D;
            break;
        case 2:
            D_800CA148 = 0x5E;
            break;
        case 3:
            D_800CA148 = 0x5F;
            break;
        case 4:
            D_800CA148 = 0x60;
            break;
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
        D_800CA148 = 0x61;
        func_8004284C();
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

void func_8014D558(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

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
            func_80062CD0(0x5FB7);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 5;
            func_80083474();
            D_800CA148 = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(5);
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
            func_8014DD28();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(5);
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
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_3 = func_8008667C(5);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 8;
            break;
        case 2:
            D_800CA148 = 9;
            break;
        case 3:
            D_800CA148 = 0xA;
            break;
        case 4:
            D_800CA148 = 0x36;
            func_8004284C();
            break;
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
        if (func_8008667C(5) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 16:
        func_8004284C();
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

void func_8014D8F0(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

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
        func_800866A0(0x5FB7);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 5;
        func_80083474();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(5);
            switch (temp_v0) {
            case 0:
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
                D_800CA148 = 6;
                break;
            }
            func_8014DD28();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = (D_800E6280.unk_03E * 3) - 0xB7;
            break;
        case 2:
            D_800CA148 = (D_800E6280.unk_03E * 3) - 0xB6;
            break;
        case 3:
            D_800CA148 = (D_800E6280.unk_03E * 3) - 0xB5;
            break;
        case 4:
            D_800CA148 = 0xE;
            break;
        }
        func_8004284C();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        func_8004284C();
        temp_v0_3 = func_8008667C(5);
        switch (temp_v0_3) {
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
            func_80042940(0x11);
            break;
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        D_800CA148 = 0x6F;
        func_8004284C();
        break;
    case 19:
        if (func_8008667C(5) == 4) {
            func_8004284C();
        } else {
            normal_date_speak();
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
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014DD28(void) {
    func_800847B8(5);
    func_80132270();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
