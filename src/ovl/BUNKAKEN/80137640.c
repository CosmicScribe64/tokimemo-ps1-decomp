#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80137640(void) {
    switch (D_80122EB8) {
    case 0:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x3B);
            return;
        case 0x60:
            func_80042908(0x3C);
            return;
        case 0x61:
            func_80042908(0x3D);
            return;
        default:
            func_80046500();
            return;
        }
        break;
    case 1:
        func_80042908(0x3E);
        return;
    case 2:
        func_80042908(0x3F);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x40);
            return;
        case 0x60:
            func_80042908(0x41);
            return;
        case 0x61:
            func_80042908(0x42);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x43);
            return;
        case 0x60:
            func_80042908(0x44);
            return;
        case 0x61:
            func_80042908(0x45);
            return;
        }
        break;
    case 5:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x47);
            return;
        }
        func_80042908(0x46);
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80137804);

void func_80137A38(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 6;
                break;
            case 2:
                D_800CA148 = 7;
                break;
            case 3:
                D_800CA148 = 8;
                break;
            }
            func_80139DEC();
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
        temp_v0_2 = func_8008667C(4);
        switch (temp_v0_2) {
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
        }
        func_8007E884();
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        if (func_8008667C(4) == 3) {
            func_8007E99C();
        } else {
            func_80042940(0xD);
        }
        break;
    case 11:
        func_8007E884();
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 14:
        func_8007E99C();
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

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80137CD8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80137FB8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80138340);

void func_801386C0(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
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
            func_80139DEC();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        func_80044750(0x202);
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xB);
        break;
    case 8:
        cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        func_8004284C();
        break;
    case 9:
        func_8007E390();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(4);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x28;
                break;
            case 2:
                D_800CA148 = 0x2B;
                break;
            case 3:
                D_800CA148 = 0x2E;
                break;
            }
            D_800CA148 += D_80122EA8;
            func_80139DEC();
        }
        break;
    case 11:
        func_80085A60();
        break;
    case 12:
        temp_v0_3 = func_8008667C(4);
        switch (temp_v0_3) {
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
        func_8004284C();
        break;
    case 13:
        func_80085A60();
        break;
    case 14:
        normal_date_bggirl_fadeout();
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

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80138A08);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80138EB8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80139184);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80139364);

void func_8013954C(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(4);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x4C;
                break;
            case 2:
            case 3:
                D_800CA148 = 0x4E;
                break;
            }
            func_80139DEC();
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
        temp_v0_2 = func_8008667C(4);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x4D;
            break;
        case 2:
        case 3:
            D_800CA148 = 0x4F;
            break;
        }
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
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80139780);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80139A6C);

void func_80139DEC(void) {
    func_800847B8(4);
    func_8013223C();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
