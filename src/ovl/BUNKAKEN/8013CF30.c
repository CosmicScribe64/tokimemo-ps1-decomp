#define MAIN_API_OVERRIDE_D_80122EB8 /* matched as u32 (main_api.h: s32) */
#include "common.h"
#include "ovl/BUNKAKEN.h"

extern u32 D_80122EB8;

void func_8013CF30(void) {
    switch (D_80122EB8) {
    case 0:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x84);
            return;
        }
        func_80042908(0x85);
        return;
    case 1:
        func_80042908(0x86);
        return;
    case 2:
        func_80042908(0x87);
        return;
    case 3:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x88);
            return;
        }
        func_80042908(0x87);
        return;
    case 4:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x89);
            return;
        }
        func_80042908(0x8A);
        return;
    case 5:
        func_80042908(0x8B);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8013D048(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E99C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            func_8013EAC8();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E99C();
        break;
    case 7:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 8:
        func_80085A60();
        break;
    case 9:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013D1F4(void) {
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
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(10);
            switch (temp_v0) {
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
            }
            func_8013EAC8();
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
        func_800866A0(0x6935);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 10;
        func_80083474();
        cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(10);
            switch (temp_v0_2) {
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
            func_8013EAC8();
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

void func_8013D4D8(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x467C, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E99C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            func_8013EAC8();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E99C();
        break;
    case 7:
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800573AC();
        func_80057390(0);
        func_8005742C(0x467C, 1);
        func_8004284C();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x6935);
        }
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8007C740();
            g->unk_F5F = 0xA;
            func_80083474();
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
        }
        break;
    case 11:
        func_8007E884();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(0xA);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                D_800CA148 = 0xE;
                break;
            case 2:
                D_800CA148 = 0xF;
                break;
            case 3:
                D_800CA148 = 0x10;
                break;
            }
            if (g->unk_03E == 0x61) {
                D_800CA148 += 3;
            }
            func_8013EAC8();
        }
        break;
    case 13:
        func_80085A60();
        break;
    case 14:
        func_8007E99C();
        break;
    case 15:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:
        func_80085A60();
        break;
    case 17:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013D82C(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (g->unk_110A) {
    case 0:
        if (D_80122EB8 == 2) {
            func_8005742C(0x40FC, 1);
        } else {
            func_8005742C(0x4105, 1);
        }
        func_8004284C();
        break;
    case 1:
        func_8007E99C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x14;
                break;
            case 2:
                D_800CA148 = 0x15;
                break;
            case 3:
                D_800CA148 = 0x16;
                break;
            }
            if (D_80122EB8 == 3) {
                D_800CA148 += 3;
            }
            func_8013EAC8();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E99C();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x6935);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        func_8007C740();
        g->unk_F5F = 0xA;
        func_80083474();
        cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            if (D_80122EB8 == 2) {
                temp_v0_2 = func_8008667C(0xA);
                switch (temp_v0_2) {                /* switch 3; irregular */
                case 0:
                case 1:
                    D_800CA148 = 0x1A;
                    break;
                case 2:
                    D_800CA148 = 0x1B;
                    break;
                case 3:
                    D_800CA148 = 0x1C;
                    break;
                }
                if (g->unk_03E == 0x61) {
                    D_800CA148 += 3;
                }
            } else {
                temp_v0_3 = func_8008667C(0xA);
                switch (temp_v0_3) {                /* switch 4; irregular */
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
            }
            func_8013EAC8();
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        func_8007E99C();
        break;
    case 14:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 15:
        func_80085A60();
        break;
    case 16:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013DBE8(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E99C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x23;
                break;
            case 2:
                D_800CA148 = 0x24;
                break;
            case 3:
                D_800CA148 = 0x25;
                break;
            }
            func_8013EAC8();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E99C();
        break;
    case 7:
        ((BunkakenModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkakenModeFlags *)&D_800E7D4F)->mode = 3;
        g->unk_720 = g->unk_1108;
        g->unk_721 = g->unk_1109;
        g->unk_722 = g->unk_110A + 1;
        main_state_set(0xD0);
        break;
    case 8:
        D_80122E98 = 1;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        func_80042908(0x8C);
        break;
    case 9:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(0xAU);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 10:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 12:
        func_800866A0(0x6935);
        break;
    case 13:
        func_8007C740();
        g->unk_F5F = 0xA;
        func_80083474();
        cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
        break;
    case 14:
        func_8007E884();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(0xA);
            switch (temp_v0_2) {                    /* switch 3; irregular */
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
            }
            func_8013EAC8();
        }
        break;
    case 16:
        func_80085A60();
        break;
    case 17:
        func_8007E5A0();
        break;
    case 18:
        ((BunkakenModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkakenModeFlags *)&D_800E7D4F)->mode = 0;
        g->unk_720 = g->unk_1108;
        g->unk_721 = g->unk_1109;
        g->unk_722 = g->unk_110A + 1;
        func_80042908(0x8D);
        break;
    case 19:
        D_80122E98 = 1;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        func_80042908(0x8C);
        break;
    case 20:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(0xAU);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 21:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 22:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 23:
        func_800866A0(0x6935);
        break;
    case 24:
        func_8007C740();
        g->unk_F5F = 0xA;
        func_80083474();
        cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
        break;
    case 25:
        func_8007E884();
        break;
    case 26:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(0xA);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x29;
                break;
            case 2:
                D_800CA148 = 0x2B;
                break;
            case 3:
                D_800CA148 = 0x2D;
                break;
            }
            if ((u8) D_800E7D4E < 3U) {
                D_800CA148 += 1;
            }
            func_8013EAC8();
        }
        break;
    case 27:
        func_80085A60();
        break;
    case 28:
        func_8007E99C();
        break;
    case 29:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 30:
        func_80085A60();
        break;
    case 31:
        ((BunkakenModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013E2C0(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x4118, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E99C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0x2DU, 0x80197000, 0x6C5F);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            func_8004284C();
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x35;
                break;
            case 2:
                D_800CA148 = 0x37;
                break;
            case 3:
                D_800CA148 = 0x39;
                break;
            }
            func_8013EAC8();
        }
        break;
    case 6:
        func_80085A60();
        break;
    case 7:
        func_8007E5A0();
        break;
    case 8:
        func_80085A60();
        break;
    case 9:
        if (func_8008667C(0xA) == 3) {
            func_80042940(0xE);
        } else {
            func_8004284C();
        }
        break;
    case 10:
        func_8007E934();
        break;
    case 11:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        func_80145530();
        break;
    case 14:
        func_800AE0A0((void *)0x80180000, 0x80197000, 0x16800);
        func_8007C740();
        g->unk_F5F = 0xD;
        func_800847B8(0xDU);
        func_80083474();
        break;
    case 15:
        func_8007E390();
        break;
    case 16:
        func_80085A60();
        break;
    case 17:
        func_8007E5A0();
        break;
    case 18:
        func_80042940(0xA);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013E578(void) {
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
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(10);
            switch (temp_v0) {
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
            func_8013EAC8();
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
        temp_v0_2 = func_8008667C(10);
        switch (temp_v0_2) {
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

void func_8013E7C4(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x467C, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E99C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            func_8013EAC8();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E99C();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x6935);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        func_8007C740();
        g->unk_F5F = 0xA;
        func_80083474();
        cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(0xA);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x45;
                break;
            case 2:
                D_800CA148 = 0x46;
                break;
            case 3:
                D_800CA148 = 0x47;
                break;
            }
            if (g->unk_03E == 0x61) {
                D_800CA148 += 3;
            }
            func_8013EAC8();
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        func_8007E99C();
        break;
    case 14:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 15:
        func_80085A60();
        break;
    case 16:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013EAC8(void) {
    func_800847B8(10);
    func_801324AC();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
