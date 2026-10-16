#define RPG_BAT_OVERRIDE_D_8015EBCC /* scalar view: the original sums three flag words with EBD4 evaluated first (T-17109) */
#include "common.h"
#include "ovl/RPG_BAT.h"

extern s32 D_8015EBCC;

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

s32 func_80147930(void) {
    s32 temp_v0;

    if (D_8015EBFC.unk_14 + D_8015EBFC.unk_10 < 0x14) {
        switch (D_8015EB9C) {
        case 0:
            D_8015EBFC.unk_10 = 0x32;
            D_8015EBFC.unk_14 = 0x32;
            break;
        case 2:
            D_8015EBFC.unk_10 = 0x32;
            D_8015EBFC.unk_14 = 0x32;
            break;
        case 3:
            D_8015EBFC.unk_10 = 0x32;
            D_8015EBFC.unk_14 = 0x32;
            break;
        case 4:
            D_8015EBFC.unk_10 = 0x32;
            D_8015EBFC.unk_14 = 0x32;
            break;
        case 5:
            D_8015EBFC.unk_10 = 0x32;
            D_8015EBFC.unk_14 = 0x32;
            break;
        case 6:
            D_8015EBFC.unk_10 = 0x32;
            D_8015EBFC.unk_14 = 0x32;
            break;
        case 10:
            D_8015EBFC.unk_10 = 0x32;
            D_8015EBFC.unk_14 = 0x32;
            break;
        case 8:
            D_8015EBFC.unk_10 = 0x32;
            D_8015EBFC.unk_14 = 0x32;
            break;
        case 1:
            D_8015EBFC.unk_10 = 0x64;
            D_8015EBFC.unk_14 = 0x64;
            break;
        case 7:
            D_8015EBFC.unk_10 = 0x32;
            D_8015EBFC.unk_14 = 0x32;
            break;
        case 9:
            D_8015EBFC.unk_10 = 0x32;
            D_8015EBFC.unk_14 = 0x32;
            break;
        case 13:
            D_8015EBFC.unk_10 = 0x64;
            D_8015EBFC.unk_14 = 0x64;
            break;
        case 14:
            D_8015EBFC.unk_10 = 0x64;
            D_8015EBFC.unk_14 = 0x64;
            break;
        }
    }
    switch (D_8015EB9C) {
    case 0:
        if (func_8013E9A8(D_8015EBFC.unk_14 + D_8015EBFC.unk_10) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return 0;
        }
        D_8015EDC8 = 0;
        D_8015EBFC.unk_14 -= 5;
        return 1;
    case 2:
        if (func_8013E9A8(D_8015EBFC.unk_14 + D_8015EBFC.unk_10) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return 0;
        }
        D_8015EBFC.unk_14 -= 5;
        return 1;
    case 3:
        if (func_8013E9A8(D_8015EBFC.unk_14 + D_8015EBFC.unk_10) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return 0;
        }
        D_8015EBFC.unk_14 -= 5;
        return 1;
    case 4:
        if (func_8013E9A8(D_8015EBFC.unk_14 + D_8015EBFC.unk_10) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return 0;
        }
        D_8015EBFC.unk_14 -= 5;
        return 1;
    case 5:
        if (func_8013E9A8(D_8015EBFC.unk_14 + D_8015EBFC.unk_10) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return 0;
        }
        D_8015EBFC.unk_14 -= 5;
        return 1;
    case 6:
        if (func_8013E9A8(D_8015EBFC.unk_14 + D_8015EBFC.unk_10) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return 0;
        }
        D_8015EBD8 += 0xA;
        D_8015EBFC.unk_14 -= 5;
        return 1;
    case 10:
        if (func_8013E9A8(D_8015EBFC.unk_14 + D_8015EBFC.unk_10) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return 0;
        }
        D_8015EBE0 = 0x32;
        D_8015EBFC.unk_14 -= 5;
        return 1;
    case 8:
        temp_v0 = func_8013E9A8(D_8015EBFC.unk_14 + D_8015EBFC.unk_10);
        if (D_8015EBDC >= 0x65) {
            return 0;
        }
        if (temp_v0 < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return 0;
        }
        D_8015EBDC = 0x168;
        D_8015EBFC.unk_14 -= 5;
        return 1;
    case 1:
        return 0x80000000;
    case 7:
        if (func_8013E9A8(D_8015EBFC.unk_14 + D_8015EBFC.unk_10) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5;
            return 0;
        }
        D_8015EBFC.unk_14 -= 5;
        if (D_8015EDC4 & 0x100000) {
            if (D_8015EBD4 + D_8015EBCC + D_8015EBD0 == 3) {
                return 0;
            }
            if (func_8013E9A8(0x64) < 0x32) {
                return 1;
            }
            return 2;
        }
        return 2;
    case 9:
        if (func_8013E9A8(D_8015EBFC.unk_14 + D_8015EBFC.unk_10) < D_8015EBFC.unk_10) {
            D_8015EBFC.unk_10 -= 5; return D_8015EDF0; /* one line: as1 then loads D_8015EDF0 before the decrement */
        }
        D_8015EBFC.unk_14 -= 5;
        if (D_8015EDF0 != 0x40000000) {
            return 1;
        }
        if (func_8013E9A8(5) == 0) {
            return 2;
        }
        return 3;
    case 13:
        if (D_8015EDEC < 8) {
            return 3;
        }
        if (func_8013E9A8(2) == 0) {
            return 2;
        }
        return 1;
    case 14:
        break;
    }
}
