#include "common.h"
#include "ovl/GEKO.h"

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013D0F0", func_8013D0F0);

void func_8013D1A0(void) {
    if (D_80122CD0 == 1) {
        func_8013D1E0();
        return;
    }
    func_80046500();
}

void func_8013D1E0(void) {
    if (D_800E7389 == 0) {
        func_8013D21C();
        return;
    }
    func_80046500();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013D0F0", func_8013D21C);

void func_8013D290(void) {
    func_80046318(0x3D, 0x801B0000, 0x8627);
    func_8013D0F0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013D0F0", func_8013D2C8);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013D0F0", func_8013D408);
