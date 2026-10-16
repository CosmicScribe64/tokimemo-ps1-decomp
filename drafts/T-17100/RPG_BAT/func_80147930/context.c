#include "common.h"
#include "ovl/RPG_BAT.h"

void func_801477E0(void) {
    s32 temp_t4;
    s32 var_a0;
    u32 sel = D_8015EB9C; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 2:
        D_8015EBB0 = func_8013EA00(0x4E2, 0x2EE) + 1;
        return;
    case 5:
        D_8015EBB0 = func_8013EA00(0x7D0, 0x3E8) + 1;
        return;
    case 3:
        if (D_8015EDC4 & 0x600000) {
            D_8015EBB0 = func_8013EA00(0x3E8, 0x1F4) + 1;
        } else {
            D_8015EBB0 = func_8013EA00(0x258, 0x12C) + 1;
            D_8015EBB4 = func_8013EA00(0x258, 0x12C) + 1;
            D_8015EBB8 = func_8013EA00(0x258, 0x12C) + 1;
        }
        temp_t4 = (s32) D_8015ED94 / 4;
        var_a0 = temp_t4;
        if (temp_t4 >= 0x65) {
            var_a0 = 0x64;
        }
        D_8015EBBC = func_8013EA00(var_a0, var_a0);
        return;
    case 4:
        D_8015EBB0 = func_8013EA00(0x4B, 0x7D) + 1;
        return;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015EBD8;
extern s32 D_8015EBE0;
extern s32 D_8015EC0C;
extern s32 D_8015EC10;
/* end bulk_m2c declarations */
s32 func_80147930(void) {
    s32 temp_v0;
    s32 var_a1;
    s32 var_v1;

    var_a1 = D_8015EC10;
    var_v1 = D_8015EC0C;
    if ((var_a1 + var_v1) < 0x14) {
        switch (D_8015EB9C) {
        case 0:
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 2:
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 3:
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 4:
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 5:
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 6:
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 10:
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 8:
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 1:
            var_v1 = 0x64;
block_16:
            var_a1 = 0x64;
            break;
        case 7:
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 9:
            var_v1 = 0x32;
            var_a1 = 0x32;
            break;
        case 13:
            var_v1 = 0x64;
            goto block_16;
        case 14:
            var_v1 = 0x64;
            goto block_16;
        }
    }
    D_8015EC0C = var_v1;
    D_8015EC10 = var_a1;
    switch (D_8015EB9C) {
    case 0:
        D_8015EC0C = var_v1;
        D_8015EC10 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EC0C) {
            D_8015EC0C -= 5;
            return;
        }
        D_8015EDC8 = 0;
        D_8015EC10 -= 5;
        return;
    case 2:
        D_8015EC0C = var_v1;
        D_8015EC10 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EC0C) {
            D_8015EC0C -= 5;
            return;
        }
        D_8015EC10 -= 5;
        return;
    case 3:
        D_8015EC0C = var_v1;
        D_8015EC10 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EC0C) {
            D_8015EC0C -= 5;
            return;
        }
        D_8015EC10 -= 5;
        return;
    case 4:
        D_8015EC0C = var_v1;
        D_8015EC10 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EC0C) {
            D_8015EC0C -= 5;
            return;
        }
        D_8015EC10 -= 5;
        return;
    case 5:
        D_8015EC0C = var_v1;
        D_8015EC10 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EC0C) {
            D_8015EC0C -= 5;
            return;
        }
        D_8015EC10 -= 5;
        return;
    case 6:
        D_8015EC0C = var_v1;
        D_8015EC10 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EC0C) {
            D_8015EC0C -= 5;
            return;
        }
        D_8015EBD8 += 0xA;
        D_8015EC10 -= 5;
    default:
        return;
    case 10:
        D_8015EC0C = var_v1;
        D_8015EC10 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EC0C) {
            D_8015EC0C -= 5;
            return;
        }
        D_8015EBE0 = 0x32;
        D_8015EC10 -= 5;
        break;
    case 8:
        D_8015EC0C = var_v1;
        D_8015EC10 = var_a1;
        temp_v0 = func_8013E9A8(var_a1 + var_v1);
        if (D_8015EBDC >= 0x65) {
            return;
        }
        if (temp_v0 < D_8015EC0C) {
            D_8015EC0C -= 5;
            return;
        }
        D_8015EBDC = 0x168;
        D_8015EC10 -= 5;
        break;
        return;
    case 7:
        D_8015EC0C = var_v1;
        D_8015EC10 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EC0C) {
            D_8015EC0C -= 5;
            return;
        }
        D_8015EC10 -= 5;
        if (D_8015EDC4 & 0x100000) {
            if ((D_8015EBD4 + *D_8015EBCC + D_8015EBD0) == 3) {
                return;
            }
            if (func_8013E9A8(0x64) < 0x32) {

            } else {
                return;
            }
        } else {
            return;
        }
        break;
    case 9:
        D_8015EC0C = var_v1;
        D_8015EC10 = var_a1;
        if (func_8013E9A8(var_a1 + var_v1) < D_8015EC0C) {
            D_8015EC0C -= 5;
            return;
        }
        D_8015EC10 -= 5;
        if (D_8015EDF0 != 0x40000000) {

        } else {
            if (func_8013E9A8(5) == 0) {
                return;
            }
            return;
        }
        break;
    case 13:
        if (D_8015EDEC < 8) {
            return;
        }
        if (func_8013E9A8(2) == 0) {
            return;
        }
        break;
    }
}
