#include "common.h"
#include "ovl/RPG_BAT.h"

u8 func_8013D780(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        D_800E6280.unk_F5F = 0xE;
        func_80046290(0x1C0000A8, 0x0F0000B7, 0xE);
        func_80044750(0x300);
        func_8004284C();
        break;
    case 1:
        func_80044750(0xE4);
        func_8004284C();
        break;
    case 2:
        if (func_800460EC() & 4) {
            func_80042808();
        }
        break;
    }
}

void func_8013D834(void) {
    if (D_800E6280.unk_110D == 0) {
        switch (D_8015EB98) {
        case 0x10000:
            func_80044890(1, 0xC3A8, 0xC38D, 0xD1FB, 0xD1AD, 0xD1A2);
            break;
        case 0x20000:
            func_80044890(1, 0xC38B, 0xC370, 0xD1FB, 0xD1AD, 0xD1A2);
            break;
        case 0x40000:
            func_80044890(1, 0xC36E, 0xC355, 0xD1FB, 0xD1AD, 0xD1A2);
            break;
        case 0x80000000:
            func_80044890(1, 0xC430, 0xC402, 0xD1FB, 0xD1AD, 0xD1A2);
            break;
        case 0x8000000:
            func_80044890(1, 0xC3C9, 0xC3AA, 0xD1FB, 0xD1AD, 0xD1A2);
            break;
        case 0x100000:
            switch (D_8015EB9C) {
            case 2:
            case 4:
            case 5:
                func_80044890(1, 0xC2E8, 0xC2B6, 0xD1FB, 0xD1AD, 0xD1A2);
                break;
            case 0:
            case 1:
            case 3:
            case 10:
                func_80044890(1, 0xC31F, 0xC2EA, 0xD1FB, 0xD1AD, 0xD1A2);
                break;
            case 6:
            case 7:
            case 8:
            case 9:
                func_80044890(1, 0xC353, 0xC321, 0xD1FB, 0xD1AD, 0xD1A2);
                break;
            }
            break;
        }
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 1) {
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        if (func_80044F94(1) == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 3) {
        if (D_8015EBA4 == 1) {
            switch (D_8015EBA0) {
            case 1:
            case 5:
            case 6:
            case 7:
            case 8:
            case 10:
                func_80044890(0, 0xC22E, 0xC211, 0, 0, 0);
                break;
            case 9:
            case 11:
                func_80044890(0, 0xC244, 0xC230, 0, 0, 0);
                break;
            case 0:
            case 3:
            case 4:
                func_80044890(0, 0xC265, 0xC246, 0, 0, 0);
                break;
            case 2:
                func_80044890(0, 0xC286, 0xC267, 0, 0, 0);
                break;
            }
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else {
            /* three single increments: ugen takes a temporary per step, a +3 leaves every later temporary two registers low */
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 4) {
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 5) {
        if (func_80044F94(0) == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else {
        func_80042808();
    }
}

/* s32 without a return: v0 live at the exit keeps as1 from filling the branch delay slots with the next block's lui,
 * as in the original (T-17109). */
s32 func_8013DC60(void) {
    if (D_800E6280.unk_110D == 0) {
        switch (D_8015EB98) {
        case 0x10000:
            cd_read_sectors(0x80, 0x80180000, 0x3A56);
            break;
        case 0x20000:
            cd_read_sectors(0x70, 0x80180000, 0x3D15);
            break;
        case 0x40000:
            cd_read_sectors(0x70, 0x80180000, 0x3BA9);
            break;
        case 0x80000000:
            cd_read_sectors(0x99, 0x80180000, 0x3DF3);
            break;
        case 0x8000000:
            cd_read_sectors(0x95, 0x80180000, 0x3C5E);
            break;
        case 0x100000:
            cd_read_sectors(0x62, 0x80180000, 0x396B);
            break;
        }
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 1) {
        if (cd_read_sync_status() & 1) {
            if (D_8015EB98 != 0x100000) {
                D_800E6280.unk_110D = 4;
            } else {
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            }
        }
    } else if (D_800E6280.unk_110D == 2) {
        cd_read_sectors(0x2B, 0x801B1000, 0x3C33);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 3) {
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 4) {
        if (D_8015EBA4 == 1) {
            switch (D_8015EBA0) {
            case 0:
                cd_read_sectors(0x21, 0x801D8000, 0x3A08);
                break;
            case 1:
                cd_read_sectors(0x21, 0x801D8000, 0x390C);
                break;
            case 2:
                cd_read_sectors(0x22, 0x801D8000, 0x3CF3);
                break;
            case 3:
                cd_read_sectors(0x2D, 0x801D8000, 0x3A29);
                break;
            case 4:
                cd_read_sectors(0x2D, 0x801D8000, 0x392D);
                break;
            case 5:
                cd_read_sectors(0x22, 0x801D8000, 0x39E6);
                break;
            case 6:
                cd_read_sectors(0x19, 0x801D8000, 0x3EB7);
                break;
            case 7:
                cd_read_sectors(0x1A, 0x801D8000, 0x3D85);
                break;
            case 8:
                cd_read_sectors(0x29, 0x801D8000, 0x3DCA);
                break;
            case 9:
                cd_read_sectors(0x2B, 0x801D8000, 0x3D9F);
                break;
            case 10:
                cd_read_sectors(0x19, 0x801D8000, 0x39CD);
                break;
            case 11:
                cd_read_sectors(0x3B, 0x801D8000, 0x3AF8);
                break;
            }
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else {
            /* two single increments: ugen takes a temporary per step, a +2 leaves every later temporary one register low */
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 5) {
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 6) {
        switch (D_8015EB9C) {
        case 0:
            cd_read_sectors(9, 0x801C7000, 0x3B33);
            break;
        case 1:
            cd_read_sectors(0x11, 0x801C7000, 0x3B6E);
            break;
        case 2:
            if (D_8015EB98 != 0x80000000) {
                cd_read_sectors(0x22, 0x801C7000, 0x3AD6);
            } else {
                func_80042808();
            }
            break;
        case 3:
            cd_read_sectors(0x19, 0x801C7000, 0x3B55);
            break;
        case 4:
            cd_read_sectors(0x11, 0x801C7000, 0x3C22);
            break;
        case 5:
            cd_read_sectors(0x11, 0x801C7000, 0x3B98);
            break;
        case 6:
            cd_read_sectors(0x19, 0x801C7000, 0x3B7F);
            break;
        case 7:
            cd_read_sectors(0x19, 0x801C7000, 0x3B3C);
            break;
        case 8:
            cd_read_sectors(0x11, 0x801C7000, 0x395A);
            break;
        case 9:
            cd_read_sectors(9, 0x801C7000, 0x3C19);
            break;
        case 10:
            cd_read_sectors(0x11, 0x801C7000, 0x3EE9);
            break;
        case 13:
            cd_read_sectors(0x19, 0x801C7000, 0x3ED0);
            break;
        case 14:
            func_80042808();
            break;
        }
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 7) {
        if (cd_read_sync_status() & 1) {
            func_80042808();
        }
    } else {
        func_80042808();
    }
}
