#include "common.h"
#include "ovl/TACO.h"

s32 func_80138890(void) {
    D_800E6280.unk_1100 += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80138B2C();
        break;
    case 1:
        func_80138D08();
        break;
    case 2:
        func_80139118();
        break;
    }
    if (D_800E6280.unk_110A < 2) {
        /* u32 view of unk_1100: the compare and the subtraction then share one load, as in the original */
        if (*(u32 *)&D_800E6280.unk_1100 < 0x32U) {
            return func_8013B460(0x64 - *(u32 *)&D_800E6280.unk_1100);
        }
        return func_8013B460(0x32);
    }
}

/* No value is returned from the reset arm on purpose: v0 then holds the target (promoted D_8015F4A4) as in the original; the other arm returns D_8015F4A8. T-16564 */
s32 func_80138964(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg3 == 0) {
        D_8015F4A4 = arg0;
        D_8015F4A8 = 0;
    } else {
        if ((D_8015F4A8 < D_8015F4A4) && !(D_800E6280.unk_10F8 & 1)) {
            if (D_8015F4A8 < (D_8015F4A4 % 10)) {
                D_8015F4A8 += 1;
            } else if (D_8015F4A8 < (D_8015F4A4 % 100)) {
                D_8015F4A8 += 10;
            } else if (D_8015F4A8 < (D_8015F4A4 % 1000)) {
                D_8015F4A8 += 100;
            } else if (D_8015F4A8 < (D_8015F4A4 % 10000)) {
                D_8015F4A8 += 1000;
            } else if (D_8015F4A8 < (D_8015F4A4 % 100000)) {
                D_8015F4A8 += 10000;
            } else if (D_8015F4A8 < (D_8015F4A4 % 1000000)) {
                D_8015F4A8 += 100000;
            } else if (D_8015F4A8 < (D_8015F4A4 % 10000000)) {
                D_8015F4A8 += 1000000;
            }
        }
        if (D_8015F4A4 < D_8015F4A8) {
            D_8015F4A8 = D_8015F4A4;
        }
        func_801430D0(D_8015F4A8, arg1, arg2, 0);
        return D_8015F4A8;
    }
}

/* NON_MATCHING: T-16564, 30 differing instruction rows (60 funcdiff lines), registers only: the original colours the u8 count D_8015EDCC $a0, the second local $a1 and the sum $a0; IDO gives $v0, $a0 and $a1. */
#ifdef NON_MATCHING
s32 func_80138B2C(void) {
    s32 var_a1;
    s32 var_v0;
    s32 var_v1;
    u8 temp_v0;

    D_800E6280.unk_037 = 0;
    D_800E6280.unk_036 = 0;
    D_8015F490[0] = D_8015EDF0 = D_8015EDF0 + (D_8015EDCC * 0xF4240);
    D_800E6280.unk_038 = 0;
    var_a1 = 0;
    if ((u8) D_8015EDCC >= 3U) {
        temp_v0 = D_8015EDB4->unk84[1];
        if (temp_v0 == 0xFF) {
            var_v1 = 0x7A120;
        } else {
            var_v1 = temp_v0 * 0x3E8;
        }
        var_a1 = D_8015EDEC * 0x64;
    } else {
        var_v1 = D_8015EDB4->unk84[1] * 0x64;
    }
    if (D_8015EDB4->unk80 != 0) {
        var_v0 = 0x7A120;
    } else if (D_8015EDB4->unk7E != 0) {
        var_v0 = 0x3D090;
    } else if (D_8015EDB4->unk7C != 0) {
        var_v0 = 0x186A0;
    } else {
        var_v0 = 0;
    }
    D_8015F490[2] = var_v0;
    D_8015F490[1] = var_v1;
    D_8015F490[4] = D_8015EDF0 + var_v1 + var_v0 + var_a1;
    D_8015F490[3] = var_a1;
    if (D_8015F490[4] >= 0x989680) {
        D_8015F490[4] = 0x98967F;
    }
    func_80044750(0x24);
    func_80059048();
    func_80143AF4();
    func_8004284C();
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80138890", func_80138B2C);
#endif

/* No return value on purpose: implicit-int function (v0 of the last call falls through). T-16564 */
s32 func_80138D08(void) {
    switch (D_800E6280.unk_110D) {
    case 8:
    case 9:
    default:
        func_801432F0("TOTAL", -0xE4, 0x48, 1);
    case 6:
    case 7:
        func_801432F0("SHIELD BONUS", -0xE4, 0x18, 5);
    case 4:
    case 5:
        func_801432F0("BOMB   BONUS", -0xE4, -8, 2);
    case 2:
    case 3:
        func_801432F0("TIME   BONUS", -0xE4, -0x28, 3);
        break;
    case 0:
    case 1:
        break;
    }
    func_801432F0("SCORE", -0xE4, -0x48, 6);
    switch (D_800E6280.unk_110D) {
    case 10:
    default:
        func_801430D0(D_8015F490[4], -0x40, 0x58, 0);
    case 8:
    case 9:
        func_801430D0(D_8015F490[1], -0x40, 0x28, 0);
    case 6:
    case 7:
        func_801430D0(D_8015F490[2], -0x40, 8, 0);
    case 4:
    case 5:
        func_801430D0(D_8015F490[3], -0x40, -0x18, 0);
    case 2:
    case 3:
        func_801430D0(D_8015F490[0], -0x40, -0x38, 0);
        break;
    case 0:
    case 1:
        break;
    }
    /* FAKE: u8 view of the selector keeps it a plain ugen temporary for the third switch; with the field it is promoted to a register (the 0xA test and the stores below). T-16564 */
    switch (*(u8 *)&D_800E6280.unk_110D) {
    case 0:
        func_80138964(D_8015F490[0], -0x40, -0x38, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_80138964(D_8015F490[0], -0x40, -0x38, 1) == D_8015F490[0]) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_80138964(D_8015F490[3], -0x40, -0x18, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        if (func_80138964(D_8015F490[3], -0x40, -0x18, 1) == D_8015F490[3]) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 4:
        func_80138964(D_8015F490[2], -0x40, 8, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        if (func_80138964(D_8015F490[2], -0x40, 8, 1) == D_8015F490[2]) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 6:
        func_80138964(D_8015F490[1], -0x40, 0x28, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        if (func_80138964(D_8015F490[1], -0x40, 0x28, 1) == D_8015F490[1]) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 8:
        func_80138964(D_8015F490[4], -0x40, 0x58, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        if (func_80138964(D_8015F490[4], -0x40, 0x58, 1) == D_8015F490[4]) {
            D_800E6280.unk_110D += 1;
        }
        break;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        if ((u8) D_800E6280.unk_110D < 0xAU) {
            D_800E6280.unk_110D = 0xA;
        } else {
            D_8015EDF0 = D_8015F490[4];
            func_80059308();
            func_8004284C();
        }
    }
}

void func_80139118(void) {
    func_800591D8(0);
    if (D_8015EE00 < D_8015EDF0) {
        func_80042908(9);
        return;
    }
    func_80042908(5);
}
