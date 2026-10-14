#include "common.h"
#include "ovl/BUNKASAI.h"

void func_80138A70(void) {
    BunkaDispatch8 *dispatch = (BunkaDispatch8 *)&D_80122EB8;
    dispatch->unk_04 = 8;
    switch (dispatch->unk_00) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x87);
            return;
        }
        func_80042908(0x86);
        return;
    case 1:
        func_80042908(0x88);
        return;
    case 2:
        func_80042908(0x89);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x8A);
            return;
        case 0x60:
            func_80042908(0x8C);
            return;
        case 0x61:
            func_80042908(0x89);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x8D);
            return;
        case 0x60:
            func_80042908(0x8E);
            return;
        case 0x61:
            func_80042908(0x8F);
            return;
        }
        break;
    case 5:
        func_80042908(0x90);
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80138A70", func_80138BE4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80138A70", func_80138EF8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80138A70", func_801391B8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80138A70", func_80139504);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80138A70", func_80139920);

void func_80139D84(void) {
    s32 temp_v0_2;
    u32 temp_v0;
    u32 temp_v0_3;
    u32 temp_v0_4;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_800866A0(0x5012);
        break;
    case 4:
        func_8007C740();
        D_800E6280.unk_F5F = 8;
        func_80083474();
        func_801431D4();
        D_800CA148 = 0x2A;
        break;
    case 5:
        func_8007E81C();
        break;
    case 6:
        func_80085A60();
        break;
    case 7:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        func_8004284C();
        break;
    case 8:
        func_8007E390();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(8);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x3D;
                break;
            case 2:
                D_800CA148 = 0x3E;
                break;
            case 3:
                D_800CA148 = 0x3F;
                break;
            case 4:
                D_800CA148 = 0x40;
                break;
            }
            func_8013B120();
        }
        break;
    case 10:
        func_80085A60();
        break;
    case 11:
        if (func_8008667C(8) == 4) {
            func_8007E5A0();
        } else {
            func_80042940(0xE);
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        func_80042940(0x19);
        break;
    case 14:
        func_8007E99C();
        break;
    case 15:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkasaiModeFlags *)&D_800E7D4F)->mode = 4;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        main_state_set(0xD0);
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
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(8U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 18:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 20:
        func_800866A0(0x5012);
        break;
    case 21:
        func_8007C740();
        D_800E6280.unk_F5F = 8;
        func_80083474();
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 22:
        func_8007E884();
        break;
    case 23:
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
            func_8013B120();
        }
        break;
    case 24:
        func_80085A60();
        break;
    case 25:
        if (func_8008667C(8) == 4) {
            func_8004284C();
        } else {
            func_8007E5A0();
        }
        break;
    case 26:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkasaiModeFlags *)&D_800E7D4F)->mode = 0;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        func_80042908(0xA4);
        break;
    case 27:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xA3);
        break;
    case 28:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(8U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 29:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 30:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 31:
        func_800866A0(0x5012);
        break;
    case 32:
        func_8007C740();
        D_800E6280.unk_F5F = 8;
        func_80083474();
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 33:
        if (func_8008667C(8) == 4) {
            func_8007E81C();
        } else {
            func_8007E884();
        }
        break;
    case 34:
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
            case 4:
                D_800CA148 = 0x4B;
                break;
            }
            if ((u8) D_800E7D4E < 4U) {
                D_800CA148 += 1;
            }
            func_8013B120();
        }
        break;
    case 35:
        func_80085A60();
        break;
    case 36:
        func_8004284C();
        temp_v0_4 = func_8008667C(8);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 0x3A;
            break;
        case 2:
            D_800CA148 = 0x3B;
            break;
        case 3:
            D_800CA148 = 0x3C;
            break;
        case 4:
            func_80042940(0x27);
            break;
        }
        break;
    case 37:
        func_80085A60();
        break;
    case 38:
        func_8007E5A0();
        break;
    case 39:
        func_8007E934();
        break;
    case 40:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 41:
        func_80085A60();
        break;
    case 42:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80138A70", func_8013A614);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80138A70", func_8013A95C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80138A70", func_8013ABA0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80138A70", func_8013AE08);

void func_8013B120(void) {
    func_800847B8(8);
    func_8013230C();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
