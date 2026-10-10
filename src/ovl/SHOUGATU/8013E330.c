#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013E330", func_8013E330);

void func_8013E410(void) {
    u8 kind;

    kind = func_80051A68(0) & 0x7F;
    if ((D_80146050 != 0) || (kind == 4)) {
        func_8013D2A8();
        return;
    }
    func_8004284C();
}

void func_8013E464(void) {
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    if (((u8)func_80051A68(D_800E71DF) & 0x7F) != 4) {
        func_8004284C();
        return;
    }
    func_8013D2A8();
}

void func_8013E4B4(void) {
    if (D_80146050 != 0) {
        if (D_80145F2C == 1) {
            D_80145F2C += 1;
        } else {
            D_80145F2C += 2;
        }
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013E330", func_8013E50C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013E330", func_8013E6E4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013E330", func_8013EA98);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013E330", func_8013EE30);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013E330", func_8013F070);
