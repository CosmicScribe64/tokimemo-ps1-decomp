#include "common.h"
#include "ovl/GEKO.h"

void func_8013D0F0(void) {
    D_80146BC0 = 0x801CE0F4;
    D_80146BC4 = 0x801CE0F8;
    D_80146BC8 = 0x801CE118;
    D_80146BCC = *(s16 *)0x801CE12C;
    D_80146BD0 = 0x801B0000;
    D_80146BD4 = 0x801B2000;
    D_80146BD8 = 0x801B6000;
    D_80146BDC = 0x801BA000;
    D_80146BE0 = 0x801BE000;
    D_80146BE4 = 0x801C2000;
    D_80146BE8 = 0x801C6000;
}

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
