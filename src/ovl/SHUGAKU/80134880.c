#include "common.h"
#include "ovl/SHUGAKU.h"

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_80134880);

void func_80134930(void) {
    if (D_800E69A1 == 3) {
        func_80134970();
        return;
    }
    func_80134B94();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_80134970);

void func_801349EC(void) {
    D_800E71DF = D_800E69DD;
    func_800847B8(D_800E69DD);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_80134A20);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_80134A8C);

void func_80134B2C(void) {
    func_80042908(D_800E69A1);
    func_80042940(D_800E69A2);
}

void func_80134B64(void) {
    func_80042908((D_800E69A1 + 1) & 0xFF);
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_80134B94);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_80134C98);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_80134D8C);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_80134DE8);

void func_80134E74(void) {
    func_80044750(0x205);
    func_8004284C();
}

void func_80134E9C(void) {
    func_80046318(0x76, 0x801B0000, 0x8D47);
    func_80134880();
    func_8004284C();
}

void func_80134ED4(void) {
    D_800E69DD = D_800E71DF;
    D_800E71DF = 0xD;
    func_800847B8(0xD);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_80134F18);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_801350F4);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_801351CC);

void func_80135278(void) {
    D_800E62BC = D_8013C1C0;
    D_800CA2DC = 0x29;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_801352B0);
