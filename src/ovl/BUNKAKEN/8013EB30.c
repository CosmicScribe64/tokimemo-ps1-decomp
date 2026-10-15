#define MAIN_API_OVERRIDE_func_80083440 /* T-12701: K&R call; callee narrows the argument. */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80083440();

void func_8013EB30(void) {
    switch (D_80122EB8) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x71);
            return;
        }
        func_80042908(0x70);
        return;
    case 1:
        func_80042908(0x72);
        return;
    case 2:
        func_80042908(0x73);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x74);
            return;
        case 0x60:
            func_80042908(0x75);
            return;
        case 0x61:
            func_80042908(0x73);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x76);
            return;
        case 0x60:
            func_80042908(0x77);
            return;
        case 0x61:
            func_80042908(0x78);
            return;
        }
        break;
    case 5:
        func_80042908(0x79);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8013EC98(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;

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
            cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(8);
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
            func_80140EF4();
            if (g->unk_03E == 0x60) {
                D_800CA148 += 3;
            }
        }
        break;
    case 5:
        func_80085A60();
        if (func_8008667C(8) < 3) {
            if (D_800CA14C >= 3) {
                func_80083440(5U);
            }
        }
        break;
    case 6:
        if (func_8008667C(8) == 3) {
            func_80042940(9);
        } else {
            D_800CA148 = 1;
            func_8007E99C();
        }
        break;
    case 7:
        func_80085A60();
        func_80083440(3U);
        break;
    case 8:
        func_8007E884();
        break;
    case 9:
        temp_v0_2 = func_8008667C(8);
        switch (temp_v0_2) {                        /* switch 3; irregular */
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
    case 10:
        func_80085A60();
        if (D_800CA14C >= 4) {
            func_80083440(func_80051B48(8) & 0x7F);
        }
        break;
    case 11:
        func_8007E5A0();
        break;
    case 12:
        func_8007E934();
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

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013EB30", func_8013EFA4);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013EB30", func_8013F29C);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013EB30", func_8013F6AC);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013EB30", func_8013F9E8);

void func_8013FD40(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;

    switch (D_800E6280.unk_110A) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(8);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x3C;
                break;
            case 2:
                D_800CA148 = 0x3E;
                break;
            case 3:
                D_800CA148 = 0x40;
                break;
            }
            func_80140EF4();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_80085A60();
        break;
    case 7:
        func_8007E99C();
        break;
    case 8:
        ((BunkakenModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkakenModeFlags *)&D_800E7D4F)->mode = 4;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        main_state_set(0xD0);
        break;
    case 9:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0x8C);
        break;
    case 10:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(8U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 11:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 13:
        func_800866A0(0x5012);
        break;
    case 14:
        func_8007C740();
        D_800E6280.unk_F5F = 8;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
        break;
    case 15:
        func_8007E884();
        break;
    case 16:
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
            func_80140EF4();
        }
        break;
    case 17:
        func_80085A60();
        break;
    case 18:
        func_8007E5A0();
        break;
    case 19:
        ((BunkakenModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkakenModeFlags *)&D_800E7D4F)->mode = 0;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        func_80042908(0x8D);
        break;
    case 20:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0x8C);
        break;
    case 21:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(8U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 22:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 24:
        func_800866A0(0x5012);
        break;
    case 25:
        func_8007C740();
        D_800E6280.unk_F5F = 8;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
        break;
    case 26:
        func_8007E884();
        break;
    case 27:
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
            }
            if ((u8) D_800E7D4E < 4U) {
                D_800CA148 += 1;
            }
            func_80140EF4();
        }
        break;
    case 28:
        func_80085A60();
        break;
    case 29:
        temp_v0_4 = func_8008667C(8);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 0x4B;
            break;
        case 2:
            D_800CA148 = 0x4C;
            break;
        case 3:
            D_800CA148 = 0x4D;
            break;
        }
        func_8004284C();
        break;
    case 30:
        func_80085A60();
        break;
    case 31:
        func_8007E5A0();
        break;
    case 32:
        func_8007E934();
        break;
    case 33:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 34:
        func_80085A60();
        break;
    case 35:
        ((BunkakenModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013EB30", func_801404B0);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013EB30", func_801406AC);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013EB30", func_80140878);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013EB30", func_80140B64);

void func_80140EF4(void) {
    func_800847B8(8);
    func_801323DC();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
