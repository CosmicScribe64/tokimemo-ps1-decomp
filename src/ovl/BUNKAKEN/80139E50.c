#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80139E50(void) {
    switch (D_80122EB8) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(7);
            return;
        }
        func_80042908(6);
        return;
    case 1:
        func_80042908(8);
        return;
    case 2:
        func_80042908(9);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0xA);
            return;
        case 0x60:
            func_80042908(0xC);
            return;
        case 0x61:
            func_80042908(0xD);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0xE);
            return;
        case 0x60:
            func_80042908(0xF);
            return;
        case 0x61:
            func_80042908(0x10);
            return;
        }
        break;
    case 5:
        func_80042908(0x11);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80139FB8(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
    case 6:
    case 10:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_8004284C();
        }
        break;
    case 3:
    case 8:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            if (D_800E6280.unk_03E == 0x5F) {
                D_800CA148 = 0x30;
            } else {
                D_800CA148 = 0x33;
            }
            func_8013B4A0();
        }
        break;
    case 5:
    case 7:
    case 9:
    case 12:
        func_80085A60();
        break;
    case 11:
        D_800CA148 = 0x52;
        func_8004284C();
        break;
    case 13:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013A118(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x467C, 1);
        func_8004284C();
        break;
    case 1:
    case 6:
    case 12:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x36;
            func_8013B4A0();
        }
        break;
    case 5:
    case 11:
    case 14:
        func_80085A60();
        break;
    case 7:
        D_80122EAC = 4;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_80145844();
        D_80125CA0 = 0xC0;
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        func_8004284C();
        break;
    case 9:
        func_8007E81C();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x37;
            func_8013B4A0();
        }
        break;
    case 13:
        D_800CA148 = 0x52;
        func_8004284C();
        break;
    case 15:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013A2FC(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x467C, 1);
        func_8004284C();
        break;
    case 1:
    case 6:
    case 13:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_8004284C();
        }
        break;
    case 3:
    case 10:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x38;
            func_8013B4A0();
        }
        break;
    case 5:
    case 12:
    case 15:
        func_80085A60();
        break;
    case 7:
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
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
            func_80145844();
            D_80125CA0 = 0xC0;
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_8004284C();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x39;
            func_8013B4A0();
        }
        break;
    case 14:
        D_800CA148 = 0x52;
        func_8004284C();
        break;
    case 16:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013A518(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x40FC, 1);
        func_8004284C();
        break;
    case 1:
    case 6:
    case 12:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_8004284C();
        }
        break;
    case 3:
    case 9:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x3A;
            func_8013B4A0();
        }
        break;
    case 5:
    case 11:
    case 14:
        func_80085A60();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_80145844();
        D_80125CA0 = 0xC0;
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        func_8004284C();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = D_800E6280.unk_03E - 0x24;
            func_8013B4A0();
        }
        break;
    case 13:
        D_800CA148 = 0x52;
        func_8004284C();
        break;
    case 15:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013A6F4(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x4105, 1);
        func_8004284C();
        break;
    case 1:
    case 10:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x3E;
            func_8013B4A0();
        }
        break;
    case 5:
    case 9:
    case 12:
        func_80085A60();
        break;
    case 6:
        func_80044750(0x202);
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xB);
        break;
    case 7:
        D_80122EBC = 0xFF;
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        func_8004284C();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = D_80122EA8 + 0x3F;
            func_8013B4A0();
        }
        break;
    case 11:
        D_800CA148 = 0x52;
        func_8004284C();
        break;
    case 13:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80139E50", func_8013A8CC);

void func_8013ABF0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x4105, 1);
        func_8004284C();
        break;
    case 1:
    case 6:
    case 12:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_8004284C();
        }
        break;
    case 3:
    case 9:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x44;
            func_8013B4A0();
        }
        break;
    case 5:
    case 11:
    case 14:
        func_80085A60();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_80145844();
        D_80125CA0 = 0xC0;
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        func_8004284C();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x45;
            func_8013B4A0();
        }
        break;
    case 13:
        D_800CA148 = 0x52;
        func_8004284C();
        break;
    case 15:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013ADC4(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x410F, 1);
        func_8004284C();
        break;
    case 1:
    case 6:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x46;
            func_8013B4A0();
        }
        break;
    case 5:
    case 8:
        func_80085A60();
        break;
    case 7:
        D_800CA148 = 0x52;
        func_8004284C();
        break;
    case 9:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80139E50", func_8013AF00);

void func_8013B170(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x410F, 1);
        func_8004284C();
        break;
    case 1:
    case 6:
    case 10:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_8004284C();
        }
        break;
    case 3:
    case 8:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x4C;
            func_8013B4A0();
        }
        break;
    case 5:
    case 7:
    case 9:
    case 12:
        func_80085A60();
        break;
    case 11:
        D_800CA148 = 0x52;
        func_8004284C();
        break;
    case 13:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013B2AC(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x467C, 1);
        func_8004284C();
        break;
    case 1:
    case 6:
    case 12:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_8004284C();
        }
        break;
    case 3:
    case 9:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x4F;
            func_8013B4A0();
        }
        break;
    case 5:
    case 11:
    case 14:
        func_80085A60();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_80145844();
        D_80125CA0 = 0xC0;
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        func_8004284C();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            if (D_800E6280.unk_03E == 0x60) {
                D_800CA148 = 0x51;
            } else {
                D_800CA148 = 0x50;
            }
            func_8013B4A0();
        }
        break;
    case 13:
        D_800CA148 = 0x52;
        func_8004284C();
        break;
    case 15:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013B4A0(void) {
    func_800847B8(0);
    func_801324E0();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
