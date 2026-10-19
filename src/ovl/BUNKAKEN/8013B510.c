#define MAIN_API_OVERRIDE_func_800847B8 /* T-24134: matched caller passes a word; callee narrows it. */
#define MAIN_API_OVERRIDE_func_80083440 /* T-24134: matched caller passes a word; callee narrows it. */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_800847B8();
void func_80083440();
s32 func_8013BD84(void);
void func_8013BFA0(void);

void func_8013B510(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        if (func_8013BD84() != 0) {
            func_8004284C();
        } else {
            func_80042908(1);
        }
        break;
    case 1:
        func_800866A0(D_80155EA0[D_80122EBC]);
        break;
    case 2:
        addr_init_bustup();
        if (D_80122EBC == 0xB) {
            D_800E6280.unk_F5F = 0;
        } else {
            D_800E6280.unk_F5F = D_80122EBC;
        }
        func_80083474();
        cd_read_sectors(D_80155F00[D_80122EBC], 0x801E8000, D_80155F0C[D_80122EBC]);
        break;
    case 3:
        normal_date_girl_in();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            if (D_80122EBC == 0xB) {
                func_800847B8(0);
            } else {
                /* FAKE: self-store preserves the original store in the call delay slot. T-24134. */
                func_800847B8(D_80122EBC = D_80122EBC);
            }
            D_80155ED0[D_80122EBC]();
            func_8013BFA0();
            D_800CA148 = 0;
            D_800CA14C = 0;
            func_8004284C();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        switch (D_80122EBC) {
        case 0:
            func_80042940(0x11);
            break;
        case 1:
        case 2:
        case 3:
            func_80042940(0x20);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            func_80042940(7);
            break;
        }
        break;
    case 7:
        normal_date_two_select();
        break;
    case 8:
        if (D_80122CDC != 0) {
            func_80083440(D_80155F3C[D_80122EBC]);
            func_80063668();
            if (D_80122EBC == 0xB) {
                D_80122EA4 = 0;
            } else {
                D_80122EA4 = D_80122EBC;
            }
        }
        func_8004284C();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (D_80122CDC == 0) {
            if ((D_80122EBC == 8) || (D_80122EBC == 9)) {
                func_80042940(0xE);
            } else {
                func_80042908(3);
            }
        } else {
            func_8004284C();
        }
        break;
    case 11:
        normal_date_girl_out();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80042908(1);
        break;
    case 14:
        D_800CA148 = 8;
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80042908(3);
        break;
    case 17:
        /* The halfword conversion preserves the original temporary order. T-24134. */
        D_800CA148 = (s16) ((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) + 1;
        func_8004284C();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        D_800E6280.unk_1BC[0].unk_0C.f.b2 = 1;
        D_800CA148 = 7;
        func_8004284C();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        normal_date_two_select();
        break;
    case 22:
        if (D_80122CDC != 0) {
            func_80083440(3);
            func_80063668();
        }
        func_8004284C();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        if (D_80122CDC != 0) {
            func_80042940(0x1C);
        } else {
            /* The halfword conversion preserves the original temporary order. T-24134. */
            D_800CA148 = (s16) ((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) + 9;
            func_8004284C();
        }
        break;
    case 25:
        normal_date_girl_out();
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        D_80122EB8 = (u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4;
        D_80122EBC = 0xFF;
        func_80042908(4);
        break;
    case 28:
        normal_date_girl_out();
        break;
    case 29:
        D_800CA148 = 0xF;
        D_80122EA4 = 0;
        func_8004284C();
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        func_80042908(1);
        break;
    case 32:
        switch (D_80122EBC) {
        case 1:
            if (((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 0) {
                D_800CA148 = 1;
            } else {
                D_800CA148 = 2;
            }
            break;
        case 2:
            if (((u8) D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 2) {
                D_800CA148 = 1;
            } else {
                D_800CA148 = 2;
            }
            break;
        case 3:
            if (((u8) D_800E6280.unk_1BC[3].unk_0C.b[2] >> 4) == 4) {
                D_800CA148 = 1;
            } else {
                D_800CA148 = 2;
            }
            break;
        default:
            func_80046500();
            break;
        }
        D_800E6280.unk_1BC[D_80122EBC].unk_0C.f.b2 = 1;
        func_8004284C();
        break;
    case 33:
        normal_date_speak();
        break;
    case 34:
        D_800CA148 = 3;
        func_8004284C();
        break;
    case 35:
        normal_date_speak();
        break;
    case 36:
        normal_date_two_select();
        break;
    case 37:
        if (D_80122CDC != 0) {
            if (D_80122EBC == 2) {
                func_80083440(4);
            } else {
                func_80083440(3);
            }
            func_80063668();
        }
        func_8004284C();
        break;
    case 38:
        normal_date_speak();
        break;
    case 39:
        if (D_80122CDC != 0) {
            func_80042940(0x2B);
        } else {
            switch (D_80122EBC) {
            case 1:
                if (((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 0) {
                    D_800CA148 = 5;
                } else {
                    D_800CA148 = 6;
                }
                break;
            case 2:
                if (((u8) D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 2) {
                    D_800CA148 = 5;
                } else {
                    D_800CA148 = 6;
                }
                break;
            case 3:
                if (((u8) D_800E6280.unk_1BC[3].unk_0C.b[2] >> 4) == 4) {
                    D_800CA148 = 5;
                } else {
                    D_800CA148 = 6;
                }
                break;
            default:
                func_80046500();
                break;
            }
            func_8004284C();
        }
        break;
    case 40:
        normal_date_girl_out();
        break;
    case 41:
        normal_date_speak();
        break;
    case 42:
        D_80122EB8 = (u8) D_800E6280.unk_1BC[D_80122EBC].unk_0C.b[2] >> 4;
        D_80122EBC = 0xFF;
        func_80042908(4);
        break;
    case 43:
        normal_date_girl_out();
        break;
    case 44:
        D_800CA148 = 7;
        D_80122EA4 = D_80122EBC;
        func_8004284C();
        break;
    case 45:
        normal_date_speak();
        break;
    case 46:
        func_80042908(1);
        break;
    default:
        func_80046500();
        break;
    }
}

s32 func_8013BD84(void) {
    s32 i;
    s32 best;
    s16 max;
    s16 v;
    u8 flags;

    best = 0;
    if ((get_h_yuukou(0) >= 0x32U) && (get_h_yuukou(0) >= 0x50U)) {
        max = D_800E6280.unk_1BC[0].unk_06 + D_800E6280.unk_1BC[0].unk_02;
    } else {
        max = 0;
    }
    flags = D_800E6280.unk_1BC[0].unk_0C.b[2];
    if (D_800E6280.unk_03E == 0x60) {
        if (((flags >> 4) == 4) || ((flags >> 4) == 1)) {
            max = 0;
        }
    }
    for (i = 1; i < 11; i++) {
        if ((get_h_tokimeki(i) >= 0x32U) && (get_h_yuukou(i) >= 0x50U) && (D_800E6280.unk_1BC[i].unk_0C.f.b1 == 1)) {
            v = D_800E6280.unk_1BC[i].unk_06 + D_800E6280.unk_1BC[i].unk_02;
            if (D_800E6280.unk_03E == 0x60) {
                if ((((u8) D_800E6280.unk_1BC[i].unk_0C.b[2] >> 4) == 4) || (((u8) D_800E6280.unk_1BC[i].unk_0C.b[2] >> 4) == 1)) {
                    v = 0;
                }
            }
            if (max < v) {
                best = i;
                max = v;
            }
        }
    }
    if ((get_h_tokimeki(best) >= 0x32U) && (get_h_yuukou(best) >= 0x50U)) {
        flags = D_800E6280.unk_1BC[0].unk_0C.b[2];
        if ((best == 0) && (D_800E6280.unk_03E == 0x60)) {
            if (((flags >> 4) == 4) || ((flags >> 4) == 1)) {
                return 0;
            }
        }
        if ((best == 0) && ((flags >> 4) >= 6U)) {
            best = 11;
        }
        D_80122EBC = best;
        return 1;
    }
    return 0;
}


void func_8013BFA0(void) {
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
}
