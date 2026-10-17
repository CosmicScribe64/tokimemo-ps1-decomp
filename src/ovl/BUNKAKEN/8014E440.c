#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_8014E440(void) {
    switch (D_80122EB8) {
    case 0:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x63);
            return;
        case 0x60:
            func_80042908(0x64);
            return;
        case 0x61:
            func_80042908(0x65);
            return;
        default:
            func_80046500();
            return;
        }
        break;
    case 1:
        func_80042908(0x66);
        return;
    case 2:
        func_80042908(0x67);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x68);
            return;
        case 0x60:
            func_80042908(0x69);
            return;
        case 0x61:
            func_80042908(0x6A);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x6B);
            return;
        case 0x60:
            func_80042908(0x6C);
            return;
        case 0x61:
            func_80042908(0x6D);
            return;
        }
        break;
    case 5:
        func_80042908(0x6E);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8014E5E0(void) {
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
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_80150CC8();
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                D_800CA148 = 3;
                break;
            case 2:
                D_800CA148 = 5;
                func_80042940(0xE);
                break;
            case 3:
                D_800CA148 = 8;
                func_80042940(0x12);
                break;
            }
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        D_800CA148 = 1;
        func_8007E99C();
        break;
    case 7:
        func_80085A60();
        break;
    case 8:
        D_800CA148 = 4;
        func_8007E884();
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        func_8007E99C();
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
        func_80085A60();
        break;
    case 15:
        D_800CA148 = 2;
        func_8007E99C();
        break;
    case 16:
        func_80085A60();
        break;
    case 17:
        D_800CA148 = 6;
        func_8007E884();
        break;
    case 18:
        func_80085A60();
        break;
    case 19:
        func_8007E5A0();
        break;
    case 20:
        func_80085A60();
        break;
    case 21:
        func_8007E934();
        break;
    case 22:
        func_80042940(0xB);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014E890(void) {
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
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_80150CC8();
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                D_800CA148 = 0xA;
                break;
            case 2:
                D_800CA148 = 0xC;
                func_80042940(9);
                break;
            case 3:
                D_800CA148 = 0xD;
                func_80042940(0xE);
                break;
            }
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        D_800CA148 = 1;
        func_8007E99C();
        break;
    case 7:
        func_80085A60();
        break;
    case 8:
        D_800CA148 = 0xB;
        func_8007E884();
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        func_8007E99C();
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
        func_80085A60();
        break;
    case 15:
        func_8007E5A0();
        break;
    case 16:
        func_80085A60();
        break;
    case 17:
        func_8007E934();
        break;
    case 18:
        func_80042940(0xB);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014EAF0(void) {
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
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                D_800CA148 = 0xF;
                break;
            case 2:
                D_800CA148 = 0x10;
                break;
            case 3:
                D_800CA148 = 0x11;
                break;
            }
            func_80150CC8();
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
        func_800866A0(0x5B25);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        func_8007C740();
        g->unk_F5F = 7;
        func_80083474();
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(7);
            switch (temp_v0_2) {                    /* switch 3; irregular */
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
            func_80150CC8();
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        if (func_8008667C(7) < 2) {
            func_80042940(0x10);
        } else {
            func_8007E5A0();
        }
        break;
    case 14:
        D_800CA148 = 0x15;
        func_8004284C();
        break;
    case 15:
        func_80085A60();
        break;
    case 16:
        if (func_8008667C(7) < 2) {
            func_8007E99C();
        } else {
            func_8007E934();
        }
        break;
    case 17:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 18:
        func_80085A60();
        break;
    case 19:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014EE54(void) {
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
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            }
            func_80150CC8();
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
            func_800866A0(0x5B25);
        }
        break;
    case 10:
        func_80145844();
        D_80125CA0 = 0xC0;
        func_80057418(1, func_8005751C(1));
        func_8007C740();
        g->unk_F5F = 7;
        func_80083474();
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        break;
    case 11:
        func_8007E884();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(7);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x19;
                break;
            case 2:
                D_800CA148 = 0x1A;
                break;
            case 3:
                D_800CA148 = 0x1B;
                break;
            }
            D_800CA148 = (D_800CA148 + (g->unk_03E * 3)) - 0x11D;
            func_80150CC8();
        }
        break;
    case 13:
        func_80085A60();
        break;
    case 14:
        if (func_8008667C(7) >= 2) {
            D_800CA148 = 0x15;
            func_8007E5A0();
        } else {
            func_8007E99C();
        }
        break;
    case 15:
        if (func_8008667C(7) < 2) {
            func_80042940(0x11);
        } else {
            func_80085A60();
        }
        break;
    case 16:
        func_8007E934();
        break;
    case 17:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 18:
        func_80085A60();
        break;
    case 19:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014F214(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x40FC, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E99C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            }
            func_80150CC8();
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
        func_800866A0(0x5B25);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        func_8007C740();
        g->unk_F5F = 7;
        func_80083474();
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(7);
            switch (temp_v0_2) {                    /* switch 3; irregular */
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
            }
            D_800CA148 = (D_800CA148 + (g->unk_03E * 3)) - 0x11D;
            func_80150CC8();
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        if (func_8008667C(7) < 2) {
            if (g->unk_03E == 0x5F) {
                func_80042940(0x15);
            } else {
                func_80042940(0x11);
            }
        } else {
            func_8004284C();
        }
        break;
    case 14:
        func_8004284C();
        break;
    case 15:
        D_800CA148 = 0x15;
        func_8007E5A0();
        break;
    case 16:
        func_80085A60();
        break;
    case 17:
        if (func_8008667C(7) < 2) {
            func_8007E99C();
        } else {
            func_8007E934();
        }
        break;
    case 18:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 19:
        func_80085A60();
        break;
    case 20:
        func_80145530();
        break;
    case 21:
        D_800CA148 = 0x2E;
        func_8007E5A0();
        break;
    case 22:
        func_80085A60();
        break;
    case 23:
        func_8007E390();
        break;
    case 24:
        func_80085A60();
        break;
    case 25:
        func_80042940(0x11);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014F624(void) {
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
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            func_80150CC8();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E5A0();
        break;
    case 7:
        func_80044750(0x202);
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        func_80042908(0xB);
        break;
    case 8:
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        func_8004284C();
        break;
    case 9:
        func_8007E390();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(7);
            switch (temp_v0_2) {                    /* switch 3; irregular */
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
            func_80150CC8();
        }
        break;
    case 11:
        func_80085A60();
        break;
    case 12:
        temp_v0_3 = func_8008667C(7);
        switch (temp_v0_3) {                        /* switch 4; irregular */
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
        func_80085A60();
        break;
    case 14:
        if (func_8008667C(7) < 2) {
            func_80042940(0x10);
        } else {
            D_800CA148 = 0x15;
            func_8007E5A0();
        }
        break;
    case 15:
        func_80085A60();
        break;
    case 16:
        if (func_8008667C(7) < 2) {
            func_8007E99C();
        } else {
            func_8007E934();
        }
        break;
    case 17:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 18:
        func_80085A60();
        break;
    case 19:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014F9D8(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;

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
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            func_80150CC8();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E5A0();
        break;
    case 7:
        ((Flags7D4F *)&D_800E7D4F)->b1 = 1;
        ((Flags7D4F *)&D_800E7D4F)->unk_2_4 = 0;
        g->unk_720 = g->unk_1108;
        g->unk_721 = g->unk_1109;
        g->unk_722 = g->unk_110A + 1;
        func_80042908(0x8D);
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
        func_800847B8(7U);
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
        func_800866A0(0x5B25);
        break;
    case 13:
        func_8007C740();
        g->unk_F5F = 7;
        func_80083474();
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        break;
    case 14:
        func_8007E884();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            func_80150CC8();
        }
        break;
    case 16:
        if (func_8008667C(7) < 2) {
            if ((u8) D_800E7D4E < 2U) {
                D_800CA148 = 0x43;
            } else {
                D_800CA148 = 0x42;
            }
            func_8004284C();
        } else {
            func_80042940(0x12);
        }
        break;
    case 17:
        func_80085A60();
        break;
    case 18:
        temp_v0_2 = func_8008667C(7);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            D_800CA148 = 0x44;
            break;
        case 2:
            D_800CA148 = 0x45;
            break;
        case 3:
            D_800CA148 = 0x46;
            break;
        }
        func_8004284C();
        break;
    case 19:
        func_80085A60();
        break;
    case 20:
        if (func_8008667C(7) < 2) {
            func_80042940(0x16);
        } else {
            D_800CA148 = 0x15;
            func_8007E5A0();
        }
        break;
    case 21:
        func_80085A60();
        break;
    case 22:
        if (func_8008667C(7) < 2) {
            func_8007E99C();
        } else {
            func_8007E934();
        }
        break;
    case 23:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 24:
        func_80085A60();
        break;
    case 25:
        ((Flags7D4F *)&D_800E7D4F)->b0 = 1;
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014FEF4(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;

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
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            }
            func_80150CC8();
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
        func_800866A0(0x5B25);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        func_8007C740();
        g->unk_F5F = 7;
        func_80083474();
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(7);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x4A;
                break;
            case 2:
                D_800CA148 = 0x4B;
                break;
            case 3:
                D_800CA148 = 0x4C;
                break;
            }
            func_80150CC8();
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        if (func_8008667C(7) < 2) {
            func_80042940(0xF);
        } else {
            D_800CA148 = 0x15;
            func_8007E5A0();
        }
        break;
    case 14:
        func_80085A60();
        break;
    case 15:
        if (func_8008667C(7) < 2) {
            func_8007E99C();
        } else {
            func_8007E934();
        }
        break;
    case 16:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 17:
        func_80085A60();
        break;
    case 18:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80150244(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E99C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x4D;
                break;
            case 2:
                D_800CA148 = 0x50;
                break;
            case 3:
                D_800CA148 = 0x53;
                break;
            }
            func_80150CC8();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E5A0();
        break;
    case 7:
        func_80085A60();
        break;
    case 8:
        func_8007E390();
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        if (func_8008667C(7) < 2) {
            func_80042940(0xC);
        } else {
            D_800CA148 = 0x15;
            func_8007E5A0();
        }
        break;
    case 11:
        func_80085A60();
        break;
    case 12:
        if (func_8008667C(7) < 2) {
            func_8007E99C();
        } else {
            func_8007E934();
        }
        break;
    case 13:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 14:
        func_80085A60();
        break;
    case 15:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8015049C(void) {
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
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x56;
                break;
            case 2:
                D_800CA148 = 0x58;
                break;
            case 3:
                D_800CA148 = 0x5A;
                break;
            }
            func_80150CC8();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        D_800E6280.unk_F5F = 0xE;
        func_8004284C();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_bg_fadeout();
        break;
    case 10:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80150680(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E99C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_80150CC8();
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x5C;
                break;
            case 2:
                D_800CA148 = 0x5E;
                break;
            case 3:
                D_800CA148 = 0x60;
                break;
            }
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        if (func_8008667C(7) < 2) {
            D_800CA148 = 1;
        } else {
            D_800CA148 = 2;
        }
        func_8007E99C();
        break;
    case 7:
        func_80085A60();
        break;
    case 8:
        temp_v0_2 = func_8008667C(7);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            D_800CA148 = 0x5D;
            break;
        case 2:
            D_800CA148 = 0x5F;
            break;
        case 3:
            D_800CA148 = 0x61;
            break;
        }
        func_8007E884();
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        if (func_8008667C(7) < 2) {
            func_80042940(0xC);
        } else {
            D_800CA148 = 0x15;
            func_8007E5A0();
        }
        break;
    case 11:
        func_80085A60();
        break;
    case 12:
        if (func_8008667C(7) < 2) {
            func_8007E99C();
        } else {
            func_8007E934();
        }
        break;
    case 13:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 14:
        func_80085A60();
        break;
    case 15:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80150958(void) {
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
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            }
            func_80150CC8();
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
        func_800866A0(0x5B25);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        func_8007C740();
        g->unk_F5F = 7;
        func_80083474();
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(7);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x65;
                break;
            case 2:
                D_800CA148 = 0x66;
                break;
            case 3:
                D_800CA148 = 0x67;
                break;
            }
            D_800CA148 = (D_800CA148 + (g->unk_03E * 3)) - 0x11D;
            func_80150CC8();
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        if (func_8008667C(7) < 2) {
            func_80042940(0xF);
        } else {
            D_800CA148 = 0x15;
            func_8007E5A0();
        }
        break;
    case 14:
        func_80085A60();
        break;
    case 15:
        if (func_8008667C(7) < 2) {
            func_8007E99C();
        } else {
            func_8007E934();
        }
        break;
    case 16:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 17:
        func_80085A60();
        break;
    case 18:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80150CC8(void) {
    func_800847B8(7);
    func_80132374();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
