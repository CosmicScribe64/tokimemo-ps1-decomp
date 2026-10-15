#define MAIN_API_OVERRIDE_D_80122EB8 /* matched as u32 (main_api.h: s32) */
#include "common.h"
#include "ovl/BUNKAKEN.h"

extern u32 D_80122EB8;

void func_8013BFE0(void) {
    D_80122EBC = 1;
    switch (D_80122EB8) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x2E);
            return;
        }
        if ((func_8008667C(1) == 4) && !D_800E6280.unk_1BC[1].unk_0C.f.b2) {
            D_80122EBC = 0xFF;
            func_80042908(5);
            return;
        }
        func_80042908(0x2D);
        return;
    case 1:
        func_80042908(0x2F);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8013C0AC(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        D_80155F50 = 0;
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x5D41);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->unk_F5F = 1;
            func_80083474();
            cd_read_sectors(7U, 0x801E8000, 0x4AB6);
        }
        break;
    case 5:
        if ((func_8008667C(1) == 4) || (D_80122EA4 == 1)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(1);
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
            if ((func_8008667C(1) == 4) || (D_80122EA4 == 1)) {
                if (g->unk_03E == 0x5F) {
                    D_800CA148 = 0xD;
                } else {
                    D_800CA148 = 0x16;
                }
            }
            func_8013CEC0();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if ((func_8008667C(1) == 4) || (D_80122EA4 == 1)) {
            func_80042940(0xC);
        } else if (!(g->unk_1BC[1].unk_0C.f.b2)) {
            g->unk_1BC[1].unk_0C.f.b2 = 1;
            D_800CA148 = 4;
            func_8004284C();
        } else {
            func_80042940(0xA);
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            if (g->unk_03E == 0x5F) {
                D_800CA148 = 7;
            } else {
                D_800CA148 = 0x10;
            }
            break;
        case 2:
            if (g->unk_03E == 0x5F) {
                D_800CA148 = 9;
            } else {
                D_800CA148 = 0x12;
            }
            break;
        case 3:
            if (g->unk_03E == 0x5F) {
                D_800CA148 = 0xB;
            } else {
                D_800CA148 = 0x14;
            }
            break;
        }
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((func_8008667C(1) == 4) || (D_80122EA4 == 1)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
            D_800CA148 = 0xE;
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((func_8008667C(1) == 4) || (D_80122EA4 == 1)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
            temp_v0_3 = func_8008667C(1);
            switch (temp_v0_3) {
            case 0:
            case 1:
                if (g->unk_03E == 0x5F) {
                    D_800CA148 = 8;
                } else {
                    D_800CA148 = 0x11;
                }
                break;
            case 2:
                if (g->unk_03E == 0x5F) {
                    D_800CA148 = 0xA;
                } else {
                    D_800CA148 = 0x13;
                }
                break;
            case 3:
                if (g->unk_03E == 0x5F) {
                    D_800CA148 = 0xC;
                } else {
                    D_800CA148 = 0x15;
                }
                break;
            }
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if ((func_8008667C(1) == 4) || (D_80122EA4 == 1)) {
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
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013C634(void) {
    GameState *g = &D_800E6280;
    u32 temp_v0;

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
            cd_read_sectors(7U, 0x801E8000, 0x4AB6);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            if (g->unk_1BC[1].unk_0C.f.b2) {
                D_800CA148 = 0x19;
            } else {
                D_800CA148 = 0x1A;
            }
            func_8013CEC0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 3;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        cd_read_sectors(7U, 0x801E8000, 0x4AB6);
        func_8004284C();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_8013CEC0();
            if (g->unk_1BC[1].unk_0C.f.b2) {
                func_80042940(0xB);
            } else {
                g->unk_1BC[1].unk_0C.f.b2 = 1;
                D_800CA148 = 4;
            }
            dec_bg_cd_read(0x40F4, 1);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        D_800CA148 = 6;
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x5D41);
            func_8004284C();
        }
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            func_801459FC();
            addr_init_bustup();
            g->unk_F5F = 1;
            func_80083474();
        }
        break;
    case 15:
        if ((func_8008667C(1) == 4) || (D_80122EA4 == 1)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        temp_v0 = func_8008667C(1);
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
        if (D_80122EA4 == 1) {
            D_800CA148 = 0x1E;
        }
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if ((func_8008667C(1) == 4) || (D_80122EA4 == 1)) {
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
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013CA6C(void) {
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
            cd_read_sectors(7U, 0x801E8000, 0x4AB6);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x1F;
            func_8013CEC0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        cd_read_sectors(7U, 0x801E8000, 0x4AB6);
        func_8004284C();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_8013CEC0();
            if (g->unk_1BC[1].unk_0C.f.b2) {
                func_80042940(0xB);
            } else {
                D_800CA148 = 5;
                g->unk_1BC[1].unk_0C.f.b2 = 1;
            }
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x40F4, 1);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        D_800CA148 = 6;
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x5D6E);
            func_8004284C();
        }
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            func_801459FC();
            addr_init_bustup();
            g->unk_F5F = 1;
            func_80083474();
        }
        break;
    case 15:
        if ((func_8008667C(1) == 4) || (D_80122EA4 == 1)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        temp_v0 = func_8008667C(1);
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
        if ((func_8008667C(1) == 4) || (D_80122EA4 == 1)) {
            D_800CA148 = 0x23;
        }
        D_800CA148 = (D_800CA148 + (g->unk_03E * 4)) - 0x17C;
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if ((func_8008667C(1) == 4) || (D_80122EA4 == 1)) {
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
        g->unk_1BC[1].unk_0C.f.b2 = 1;
        func_8004284C();
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

void func_8013CEC0(void) {
    func_800847B8(1);
    func_80132104();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
