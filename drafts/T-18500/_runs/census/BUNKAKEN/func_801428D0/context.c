#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_801428D0(void) {
    D_80122EBC = 3;
    switch (D_80122EB8) {
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            if (func_8008667C(3) == 4 && !D_800E6280.unk_1BC[3].unk_0C.f.b2) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x36);
            return;
        case 0x60:
            if (func_8008667C(3) == 4 && !D_800E6280.unk_1BC[3].unk_0C.f.b2) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x37);
            return;
        case 0x61:
            if (func_8008667C(3) == 4 && !D_800E6280.unk_1BC[3].unk_0C.f.b2) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x38);
            return;
        }
        break;
    case 5:
        func_80042908(0x39);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80142A40(void) {
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
        if (cd_read_sync_status() & 1) {
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
        func_800866A0(0x58DC);
        break;
    case 6:
        func_800AE0A0((void *)0x801AE000, 0x80180000, 0x16800);
        addr_init_bustup();
        g->unk_F5F = 3;
        func_80083474();
        func_80145844();
        cd_read_sectors(9U, 0x801E8000, 0x4B0A);
        break;
    case 7:
        normal_date_bg_fadein();
        break;
    case 8:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_80143D68();
            temp_v0 = func_8008667C(3);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 4;
                break;
            case 2:
                D_800CA148 = 0xD;
                break;
            case 3:
                D_800CA148 = 0x16;
                break;
            }
            if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
                D_800CA148 = 0x1F;
            }
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (g->unk_1BC[3].unk_0C.f.b2) {
            func_80042940(0xD);
        } else {
            D_800CA148 = 1;
            g->unk_1BC[3].unk_0C.f.b2 = 1;
            func_8004284C();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(3);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 5;
            break;
        case 2:
            D_800CA148 = 0xE;
            break;
        case 3:
            D_800CA148 = 0x17;
            break;
        }
        func_8004284C();
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            func_80042940(0x24);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        func_800847B8(0xCU);
        D_80122D20 = 1;
        g->unk_F5F = 0xC;
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
        normal_date_girl_out();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        normal_date_girl_in();
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
        g->unk_F5F = 3;
        func_80083474();
        func_800847B8(3U);
        break;
    case 29:
        normal_date_girl_in();
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        func_800847B8(0xCU);
        g->unk_F5F = 0xC;
        D_80122D20 = 1;
        func_8004284C();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        func_800847B8(3U);
        g->unk_F5F = 3;
        D_80122D20 = 0;
        func_8004284C();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        normal_date_girl_out();
        break;
    case 36:
        normal_date_bg_fadeout();
        break;
    case 37:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 38:
        normal_date_speak();
        break;
    case 39:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80142FD4(void) {
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
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(9U, 0x801E8000, 0x4B0A);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(3);
            switch (temp_v0) {
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
            }
            if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
                D_800CA148 = 0x23;
                g->unk_F5F = 0xD;
            } else {
                g->unk_F5F = 3;
            }
            func_80143D68();
            if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
                func_800847B8(0xDU);
                func_80062CD0(0x6C5F);
            } else {
                func_80062CD0(0x58DC);
            }
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
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
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            D_800CA148 = 0x2C;
        } else if (g->unk_1BC[3].unk_0C.f.b2) {
            D_800CA148 = 0x24;
        } else {
            g->unk_1BC[3].unk_0C.f.b2 = 1;
            D_800CA148 = 0x25;
        }
        func_8004284C();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            func_80042940(0x13);
        } else {
            func_8004284C();
        }
        break;
    case 11:
        temp_v0_2 = func_8008667C(3);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x26;
            break;
        case 2:
            D_800CA148 = 0x28;
            break;
        case 3:
            D_800CA148 = 0x2A;
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
        normal_date_speak();
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        D_800CA148 = 0;
        g->unk_1BC[3].unk_0C.f.b2 = 1;
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80145530();
        break;
    case 19:
        normal_date_two_select();
        break;
    case 20:
        if (D_80122CDC != 0) {
            D_800CA148 = 0x2F;
        } else {
            D_800CA148 = 0x2D;
        }
        func_8004284C();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        normal_date_girl_out();
        break;
    case 23:
        if (D_80122CDC != 0) {
            func_80042940(0xF);
        } else {
            func_800847B8(3U);
            func_8004284C();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        func_80042940(0xF);
        break;
    default:
        func_80046500();
        break;
    }
}

/* T-12230: existing CharFlags bitfields reproduce the target register choices. */
void func_80143494(void) {
    s32 temp_v0;
    s32 temp_v0_2;

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
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x58DC);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 3;
            func_80083474();
            cd_read_sectors(9U, 0x801E8000, 0x4B0A);
        }
        break;
    case 5:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_80143D68();
            temp_v0 = func_8008667C(3);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x30;
                break;
            case 2:
                D_800CA148 = 0x34;
                break;
            case 3:
                D_800CA148 = 0x38;
                break;
            }
            if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
                D_800CA148 = 0x3C;
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            func_80042940(0xC);
        } else if (D_800E6280.unk_1BC[3].unk_0C.f.b2) {
            func_80042940(0xA);
        } else {
            D_800CA148 = 1;
            func_8004284C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(3);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x31;
            break;
        case 2:
            D_800CA148 = 0x35;
            break;
        case 3:
            D_800CA148 = 0x39;
            break;
        }
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_girl_out();
        }
        break;
    case 17:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            func_8004284C();
        } else {
            normal_date_bg_fadeout();
        }
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

void func_801438F8(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(9U, 0x801E8000, 0x4B0A);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x3F;
            func_80143D68();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 4;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        cd_read_sectors(9U, 0x801E8000, 0x4B0A);
        func_8004284C();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x58AF);
        }
        break;
    case 10:
        func_80145A30();
        func_80143D68();
        if (g->unk_1BC[3].unk_0C.f.b2) {
            func_80042940(0xC);
        } else {
            g->unk_1BC[3].unk_0C.f.b2 = 1;
            D_800CA148 = 2;
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        D_800CA148 = 3;
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        addr_init_bustup();
        g->unk_F5F = 3;
        func_80083474();
        dec_bg_cd_read(0x40F4, 1);
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            func_801459FC();
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 16:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 17:
        temp_v0 = func_8008667C(3);
        switch (temp_v0) {
        case 0:
        case 1:
            D_800CA148 = (g->unk_03E * 3) - 0xDD;
            break;
        case 2:
            D_800CA148 = (g->unk_03E * 3) - 0xDC;
            break;
        case 3:
            D_800CA148 = (g->unk_03E * 3) - 0xDB;
            break;
        }
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            D_800CA148 = 0x49;
        }
        func_8004284C();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_girl_out();
        }
        break;
    case 20:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            func_8004284C();
        } else {
            normal_date_bg_fadeout();
        }
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

void func_80143D68(void) {
    func_800847B8(3);
    func_801321D4();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
