#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80138B40(void) {
    switch (D_80144E14) {
    case 0:
        func_80138BA0();
        return;
    case 1:
        func_80138C1C();
        return;
    default:
        func_80138CA4();
        return;
    }
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80138B40", func_80138BA0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80138B40", func_80138C1C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80138B40", func_80138CA4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80138B40", func_80138D2C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80138B40", func_80138E1C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80138B40", func_80138E60);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80138B40", func_80138ED4);

void func_80138F48(void) {
    bg_read_sub2(0x4071);
    func_8004284C();
}

void func_80138F70(void) {
    bg_read_sub2(0x40E2);
    func_8004284C();
}

void func_80138F98(void) {
    bg_read_sub2(0x40D9);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80138B40", func_80138FC0);
