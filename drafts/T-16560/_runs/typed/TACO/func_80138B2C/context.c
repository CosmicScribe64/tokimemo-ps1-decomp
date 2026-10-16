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

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015F490;
extern s32 D_8015F494;
extern s32 D_8015F498;
extern s32 D_8015F49C;
extern s32 D_8015F4A0;
/* end bulk_m2c declarations */
s32 func_80138B2C(void) {
    s32 temp_a0;
    s32 temp_t8;
    s32 var_a1;
    s32 var_v0;
    s32 var_v1;
    u8 temp_v0;

    D_800E6280.unk_037 = 0;
    D_800E6280.unk_036 = 0;
    temp_t8 = D_8015EDF0 + (D_8015EDCC * 0xF4240);
    D_8015EDF0 = temp_t8;
    D_8015F490 = temp_t8;
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
    } else {
        var_v0 = 0;
        if (D_8015EDB4->unk7C != 0) {
            var_v0 = 0x186A0;
        }
    }
    D_8015F498 = var_v0;
    D_8015F494 = var_v1;
    temp_a0 = D_8015EDF0 + var_v1 + var_v0 + var_a1;
    D_8015F4A0 = temp_a0;
    D_8015F49C = var_a1;
    if (temp_a0 >= 0x989680) {
        D_8015F4A0 = 0x98967F;
    }
    func_80044750(0x24);
    func_80059048();
    func_80143AF4();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80138890", func_80138D08);

void func_80139118(void) {
    func_800591D8(0);
    if (D_8015EE00 < D_8015EDF0) {
        func_80042908(9);
        return;
    }
    func_80042908(5);
}
