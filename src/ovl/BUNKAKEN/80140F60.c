#include "common.h"
#include "ovl/BUNKAKEN.h"

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80140F60", func_80140F60);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80140F60", func_80141088);

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

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80140F60", func_80141B80);

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
