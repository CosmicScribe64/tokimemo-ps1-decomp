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
