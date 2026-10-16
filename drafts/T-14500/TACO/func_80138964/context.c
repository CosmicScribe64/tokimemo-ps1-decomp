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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7378;
extern s32 D_8015F4A4;
extern s32 D_8015F4A8;
/* end bulk_m2c declarations */
s32 func_80138964(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 var_a0;
    s32 var_at;

    if (arg3 == 0) {
        D_8015F4A4 = arg0;
        D_8015F4A8 = 0;
        return arg0;
    }
    var_a0 = D_8015F4A8;
    if ((var_a0 < D_8015F4A4) && !(D_800E7378 & 1)) {
        if (var_a0 < ((s32) D_8015F4A4 % 10)) {
            var_a0 += 1;
            goto block_22;
        }
        if (var_a0 < ((s32) D_8015F4A4 % 100)) {
            var_a0 += 0xA;
            goto block_22;
        }
        if (var_a0 < ((s32) D_8015F4A4 % 1000)) {
            var_a0 += 0x64;
            goto block_22;
        }
        if (var_a0 < ((s32) D_8015F4A4 % 10000)) {
            var_a0 += 0x3E8;
            goto block_22;
        }
        if (var_a0 < ((s32) D_8015F4A4 % 100000)) {
            var_a0 += 0x2710;
            goto block_22;
        }
        if (var_a0 < ((s32) D_8015F4A4 % 1000000)) {
            var_a0 += 0x186A0;
            goto block_22;
        }
        var_at = D_8015F4A4 < var_a0;
        if (var_a0 < ((s32) D_8015F4A4 % 10000000)) {
            var_a0 += 0xF4240;
            goto block_22;
        }
    } else {
block_22:
        var_at = D_8015F4A4 < var_a0;
    }
    if (var_at != 0) {
        var_a0 = D_8015F4A4;
    }
    D_8015F4A8 = var_a0;
    func_801430D0(var_a0, arg1, arg2, 0);
    return D_8015F4A8;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80138890", func_80138B2C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80138890", func_80138D08);

void func_80139118(void) {
    func_800591D8(0);
    if (D_8015EE00 < D_8015EDF0) {
        func_80042908(9);
        return;
    }
    func_80042908(5);
}
