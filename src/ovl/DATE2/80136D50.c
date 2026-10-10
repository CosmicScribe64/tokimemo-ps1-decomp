#include "common.h"
#include "ovl/DATE2.h"

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80136D50", func_80136D50);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80136D50", func_80136FC4);

void func_80137064(void) {
    func_80046318(3, 0x801B0000, 0xAF43);
    func_80136D50();
    func_8004284C();
}

void func_8013709C(void) {
    D_800CA148 = 1;
    D_800CA14C = 0;
    func_800847B8(D_800E71DF);
    func_801370E4();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80136D50", func_801370E4);
