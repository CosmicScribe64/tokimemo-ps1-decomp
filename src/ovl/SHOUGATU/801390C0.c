#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801390C0", func_801390C0);

void func_801391D4(void) {
    func_801391F4();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801390C0", func_801391F4);

void func_801392A4(void) {
    k_reset(1);
    func_8004284C();
}

void func_801392CC(void) {
    func_80046318(0x71, 0x801B0000, 0x85B6);
    func_801390C0();
    func_8004284C();
}

void func_80139304(void) {
    func_80044750(0x24);
    func_80044750(0x501);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801390C0", func_80139334);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801390C0", func_80139514);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801390C0", func_8013960C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801390C0", func_80139734);

void func_80139778(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_801397A0(void) {
    bg_read_sub2(0x46F0);
    func_8004284C();
}
