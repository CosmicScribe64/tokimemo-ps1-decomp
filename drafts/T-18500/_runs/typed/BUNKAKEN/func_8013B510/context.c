#include "common.h"
#include "ovl/BUNKAKEN.h"

/* bulk_m2c declarations (T-11190) */
void func_8013BFA0(void);
/* end bulk_m2c declarations */
void func_8013B510(void) {
    s32 temp_a0;

    switch (D_800E6280.unk_110A) {
    case 0:
        if (func_8013BD84() != 0) {
            func_8004284C();
            return;
        }
        func_80042908(1);
        return;
    case 1:
        func_800866A0(D_80155EA0[D_80122EBC]);
        return;
    case 2:
        addr_init_bustup();
        if (D_80122EBC == 0xB) {
            D_800E6280.unk_F5F = 0;
        } else {
            D_800E6280.unk_F5F = (u8) D_80122EBC;
        }
        func_80083474();
        cd_read_sectors(D_80155F00[D_80122EBC], 0x801E8000, D_80155F0C[D_80122EBC]);
        return;
    case 3:
        normal_date_girl_in();
        return;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_a0 = D_80122EBC;
            if (D_80122EBC == 0xB) {
                func_800847B8(0U);
            } else {
                D_80122EBC = D_80122EBC;
                func_800847B8((u8) temp_a0);
            }
            D_80155ED0[D_80122EBC]();
            func_8013BFA0();
            D_800CA148 = 0;
            D_800CA14C = 0;
            func_8004284C();
            return;
        }
    default:
        return;
    case 5:
        normal_date_speak();
        return;
    case 6:
        switch (D_80122EBC) {
        case 0:
            func_80042940(0x11);
            return;
        case 1:
        case 2:
        case 3:
            func_80042940(0x20);
            return;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            func_80042940(7);
            return;
        }
        break;
    case 7:
        normal_date_two_select();
        return;
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
        return;
    case 9:
        normal_date_speak();
        return;
    case 10:
        if (D_80122CDC == 0) {
            if ((D_80122EBC == 8) || (D_80122EBC == 9)) {
                func_80042940(0xE);
                return;
            }
            func_80042908(3);
            return;
        }
        func_8004284C();
        return;
    case 11:
        normal_date_girl_out();
        return;
    case 12:
        normal_date_speak();
        return;
    case 13:
        func_80042908(1);
        return;
    case 14:
        D_800CA148 = 8;
        func_8004284C();
        return;
    case 15:
        normal_date_speak();
        return;
    case 16:
        func_80042908(3);
        return;
    case 17:
        D_800CA148 = ((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) + 1;
        func_8004284C();
        return;
    case 18:
        normal_date_speak();
        return;
    case 19:
        D_800E6280.unk_1BC[0].unk_0C.b[0] |= 4;
        D_800CA148 = 7;
        func_8004284C();
        return;
    case 20:
        normal_date_speak();
        return;
    case 21:
        normal_date_two_select();
        return;
    case 22:
        if (D_80122CDC != 0) {
            func_80083440(3U);
            func_80063668();
        }
        func_8004284C();
        return;
    case 23:
        normal_date_speak();
        return;
    case 24:
        if (D_80122CDC != 0) {
            func_80042940(0x1C);
            return;
        }
        D_800CA148 = ((u8) D_800E6280.unk0 >> 4) + 9;
        func_8004284C();
        return;
    case 25:
        normal_date_girl_out();
        return;
    case 26:
        normal_date_speak();
        return;
    case 27:
        D_80122EB8 = (s32) ((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4);
        D_80122EBC = 0xFF;
        func_80042908(4);
        return;
    case 28:
        normal_date_girl_out();
        return;
    case 29:
        D_800CA148 = 0xF;
        D_80122EA4 = 0;
        func_8004284C();
        return;
    case 30:
        normal_date_speak();
        return;
    case 31:
        func_80042908(1);
        return;
    case 32:
        switch (D_80122EBC) {
        case 1:
            if (((u8) D_800E6280.unk0 >> 4) == 0) {
                D_800CA148 = 1;
            } else {
                D_800CA148 = 2;
            }
            break;
        case 2:
            if (((u8) D_800E6280.unk0 >> 4) == 2) {
                D_800CA148 = 1;
            } else {
                D_800CA148 = 2;
            }
            break;
        case 3:
            if (((u8) D_800E6280.unk0 >> 4) == 4) {
                D_800CA148 = 1;
            } else {
                D_800CA148 = 2;
            }
            break;
        default:
            func_80046500();
            break;
        }
        D_800E6280.unk_1BC[D_80122EBC].unk_0C.b[0] |= 4;
        func_8004284C();
        return;
    case 33:
        normal_date_speak();
        return;
    case 34:
        D_800CA148 = 3;
        func_8004284C();
        return;
    case 35:
        normal_date_speak();
        return;
    case 36:
        normal_date_two_select();
        return;
    case 37:
        if (D_80122CDC != 0) {
            if (D_80122EBC == 2) {
                func_80083440(4U);
            } else {
                func_80083440(3U);
            }
            func_80063668();
        }
        func_8004284C();
        return;
    case 38:
        normal_date_speak();
        return;
    case 39:
        if (D_80122CDC != 0) {
            func_80042940(0x2B);
            return;
        }
        switch (D_80122EBC) {
        case 1:
            if (((u8) D_800E6280.unk0 >> 4) == 0) {
                D_800CA148 = 5;
            } else {
                D_800CA148 = 6;
            }
            break;
        case 2:
            if (((u8) D_800E6280.unk0 >> 4) == 2) {
                D_800CA148 = 5;
            } else {
                D_800CA148 = 6;
            }
            break;
        case 3:
            if (((u8) D_800E6280.unk0 >> 4) == 4) {
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
        return;
    case 40:
        normal_date_girl_out();
        return;
    case 41:
        normal_date_speak();
        return;
    case 42:
        D_80122EB8 = (s32) ((u8) *((u8 *)&D_800E6280 + (D_80122EBC * 0x38)) >> 4);
        D_80122EBC = 0xFF;
        func_80042908(4);
        return;
    case 43:
        normal_date_girl_out();
        return;
    case 44:
        D_800CA148 = 7;
        D_80122EA4 = D_80122EBC;
        func_8004284C();
        return;
    case 45:
        normal_date_speak();
        return;
    case 46:
        func_80042908(1);
        return;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013B510", func_8013BD84);

void func_8013BFA0(void) {
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
}
