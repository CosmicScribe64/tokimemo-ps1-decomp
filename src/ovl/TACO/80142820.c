#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80142820", func_80142820);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80142820", func_801428DC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80142820", func_80142AF0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80142820", func_80142DE0);

void func_80142EC4(void) {
    func_80059E00();
}

void func_80142EE4(void) {
    func_80059BE8();
    func_8004284C();
}

void func_80142F0C(void) {
    func_8004ADE4();
    func_8004284C();
}

u8 func_80142F34(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80046318(0x95, 0x801A0000, 0x9B94);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_800460CC() & 1) {
            func_80068938(D_800E6280.unk_03E, D_800E6280.unk_03F, 0);
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_8004284C();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80142820", func_80142FE8);
