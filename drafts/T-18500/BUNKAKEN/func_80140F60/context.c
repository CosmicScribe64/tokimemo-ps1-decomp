#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80140F60(void) {
    D_80122EBC = 2;
    switch (D_80122EB8) {
    case 2:
        if ((func_8008667C(2) == 4) && !(D_800E6280.unk_1BC[2].unk_0C.w & 4)) {
            D_80122EBC = 0xFF;
            func_80042908(5);
            return;
        }
        func_80042908(0x31);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x32);
            break;
        case 0x60:
            func_80042908(0x33);
            break;
        case 0x61:
            func_80042908(0x34);
            break;
        }
        if ((func_8008667C(2) == 4) && !(D_800E6280.unk_1BC[2].unk_0C.w & 4)) {
            D_80122EBC = 0xFF;
            func_80042908(5);
            return;
        }
        return;
    default:
        func_80046500();
        break;
    }
}

void func_80141088(void) {
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
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x525B);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->unk_F5F = 2;
            func_80083474();
            cd_read_sectors(9U, 0x801E8000, 0x4A38);
        }
        break;
    case 5:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_80142864();
            temp_v0 = func_8008667C(2);
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
                func_80042940(0xA);
                break;
            }
            if (D_80122EA4 == 2) {
                func_80042940(0xA);
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->unk_1BC[2].unk_0C.f.b2) {
            func_80042940(0xA);
        } else {
            D_800CA148 = 4;
            g->unk_1BC[2].unk_0C.f.b2 = 1;
            func_8004284C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(2);
        switch (temp_v0_2) {
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
        if (D_80122EA4 == 2) {
            D_800CA148 = 8;
        }
        D_800CA148 = (D_800CA148 + (g->unk_03E * 8)) - 0x2F8;
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            D_80122EAC = 1;
        } else {
            D_80122EAC = 3;
        }
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 14:
        func_800866A0(0x525B);
        break;
    case 15:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->unk_F5F = 2;
        func_80083474();
        cd_read_sectors(9U, 0x801E8000, 0x4A38);
        break;
    case 16:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(2);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 9;
                break;
            case 2:
                D_800CA148 = 0xA;
                break;
            case 3:
                D_800CA148 = 0xB;
                break;
            case 4:
                D_800CA148 = 0xC;
                break;
            }
            if (D_80122EA4 == 2) {
                D_800CA148 = 0xC;
            }
            D_800CA148 = (D_800CA148 + (g->unk_03E * 8)) - 0x2F8;
            func_80142864();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
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

void func_80141654(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0_3;
    u32 temp_v0;
    u32 temp_v0_2;

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
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x522E);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->unk_F5F = 2;
            func_80083474();
            cd_read_sectors(9U, 0x801E8000, 0x4A38);
        }
        break;
    case 5:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_80142864();
            temp_v0 = func_8008667C(2);
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
                func_80042940(0xA);
                break;
            }
            if (D_80122EA4 == 2) {
                func_80042940(0xA);
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->unk_1BC[2].unk_0C.f.b2) {
            func_80042940(0xA);
        } else {
            g->unk_1BC[2].unk_0C.f.b2 = 1;
            D_800CA148 = 4;
            func_8004284C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(2);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x1D;
            break;
        case 2:
            D_800CA148 = 0x1E;
            break;
        case 3:
            D_800CA148 = 0x1F;
            break;
        case 4:
            D_800CA148 = 0x20;
            break;
        }
        if (D_80122EA4 == 2) {
            D_800CA148 = 0x20;
        }
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (func_8008667C(2) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 13:
        func_80044750(0x202);
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        func_80042908(0xB);
        break;
    case 14:
        cd_read_sectors(9U, 0x801E8000, 0x4A38);
        func_8004284C();
        break;
    case 15:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            func_80142864();
            temp_v0_3 = func_8008667C(2);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x21;
                break;
            case 2:
                D_800CA148 = 0x22;
                break;
            case 3:
                D_800CA148 = 0x23;
                break;
            }
            if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
                D_800CA148 = D_80122EA8 + 0x24;
            }
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 19:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 20:
        normal_date_bg_fadeout();
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

void func_80141B80(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;

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
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x522E);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->unk_F5F = 2;
            func_80083474();
            cd_read_sectors(9U, 0x801E8000, 0x4A38);
        }
        break;
    case 5:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(2);
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
            if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
                D_800CA148 = 0x2A;
            }
            func_80142864();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->unk_1BC[2].unk_0C.f.b2) {
            func_80042940(0xA);
        } else {
            g->unk_1BC[2].unk_0C.f.b2 = 1;
            D_800CA148 = 4;
            func_8004284C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(2);
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
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            func_80042940(0xC);
        } else {
            func_8004284C();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            func_8004284C();
        } else {
            normal_date_girl_out();
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
        message_window_init();
        hizuke_init();
        func_80084E4C();
        func_800847B8(8U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 16:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            func_8004284C();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 18:
        func_800866A0(0x522E);
        break;
    case 19:
        addr_init_bustup();
        g->unk_F5F = 2;
        func_80083474();
        cd_read_sectors(9U, 0x801E8000, 0x4A38);
        break;
    case 20:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 21:
        if (cd_read_sync_status() & 1) {
            func_80142864();
            temp_v0_3 = func_8008667C(2);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x2B;
                break;
            case 2:
                D_800CA148 = 0x2D;
                break;
            case 3:
                D_800CA148 = 0x2F;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                D_800CA148 += 1;
            }
            if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
                D_800CA148 = 0x34;
            }
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        func_8004284C();
        temp_v0_4 = func_8008667C(2);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 0x31;
            break;
        case 2:
            D_800CA148 = 0x32;
            break;
        case 3:
            D_800CA148 = 0x33;
            break;
        }
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            func_80042940(0x1A);
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        normal_date_girl_out();
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
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801422C4(void) {
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
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x522E);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->unk_F5F = 2;
            func_80083474();
            cd_read_sectors(9U, 0x801E8000, 0x4A38);
        }
        break;
    case 5:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_80142864();
            temp_v0 = func_8008667C(2);
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
            if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
                func_80042940(0xA);
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->unk_1BC[2].unk_0C.f.b2) {
            func_80042940(0xA);
        } else {
            D_800CA148 = 4;
            g->unk_1BC[2].unk_0C.f.b2 = 1;
            func_8004284C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(2);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x35;
            break;
        case 2:
            D_800CA148 = 0x36;
            break;
        case 3:
            D_800CA148 = 0x37;
            break;
        }
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            D_800CA148 = 0x38;
        }
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            D_80122EAC = 1;
        } else {
            D_80122EAC = 3;
        }
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 14:
        func_800866A0(0x522E);
        break;
    case 15:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->unk_F5F = 2;
        func_80083474();
        cd_read_sectors(9U, 0x801E8000, 0x4A38);
        break;
    case 16:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(2);
            switch (temp_v0_3) {
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
            }
            if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
                D_800CA148 = 0x3C;
            }
            func_80142864();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) {
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

void func_80142864(void) {
    func_800847B8(2);
    func_8013216C();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
