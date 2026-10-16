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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80138890", func_80138964);

#ifdef NON_MATCHING
s32 func_80138B2C(void) {
    s32 var_a1;
    s32 var_v0;
    s32 var_v1;
    u8 temp_v0;

    D_800E6280.unk_037 = 0;
    D_800E6280.unk_036 = 0;
    D_8015F490 = D_8015EDF0 = D_8015EDF0 + (D_8015EDCC * 0xF4240);
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
    D_8015F498 = var_v0;
    D_8015F494 = var_v1;
    D_8015F4A0 = D_8015EDF0 + var_v1 + var_v0 + var_a1;
    D_8015F49C = var_a1;
    if (D_8015F4A0 >= 0x989680) {
        D_8015F4A0 = 0x98967F;
    }
    func_80044750(0x24);
    func_80059048();
    func_80143AF4();
    func_8004284C();
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80138890", func_80138B2C);
#endif

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
        func_801430D0(D_8015F4A0, -0x40, 0x58, 0);
    case 8:
    case 9:
        func_801430D0(D_8015F494, -0x40, 0x28, 0);
    case 6:
    case 7:
        func_801430D0(D_8015F498, -0x40, 8, 0);
    case 4:
    case 5:
        func_801430D0(D_8015F49C, -0x40, -0x18, 0);
    case 2:
    case 3:
        func_801430D0(D_8015F490, -0x40, -0x38, 0);
        break;
    case 0:
    case 1:
        break;
    }
    /* FAKE: u8 view of the selector keeps it a plain ugen temporary for the third switch; with the field it is promoted to a register (the 0xA test and the stores below). T-16564 */
    switch (*(u8 *)&D_800E6280.unk_110D) {
    case 0:
        func_80138964(D_8015F490, -0x40, -0x38, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_80138964(D_8015F490, -0x40, -0x38, 1) == D_8015F490) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_80138964(D_8015F49C, -0x40, -0x18, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        if (func_80138964(D_8015F49C, -0x40, -0x18, 1) == D_8015F49C) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 4:
        func_80138964(D_8015F498, -0x40, 8, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        if (func_80138964(D_8015F498, -0x40, 8, 1) == D_8015F498) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 6:
        func_80138964(D_8015F494, -0x40, 0x28, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        if (func_80138964(D_8015F494, -0x40, 0x28, 1) == D_8015F494) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 8:
        func_80138964(D_8015F4A0, -0x40, 0x58, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        if (func_80138964(D_8015F4A0, -0x40, 0x58, 1) == D_8015F4A0) {
            D_800E6280.unk_110D += 1;
        }
        break;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        if ((u8) D_800E6280.unk_110D < 0xAU) {
            D_800E6280.unk_110D = 0xA;
        } else {
            D_8015EDF0 = D_8015F4A0;
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
