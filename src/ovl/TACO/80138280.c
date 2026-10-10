#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80138280", func_80138280);

void func_80138348(void) {
    D_8015EDBC = 0x14;
    D_8015EDC0 = 0;
    D_8015EDB0 = 0x80;
    func_8015ABF0();
    func_8004284C();
}

void func_8013838C(void) {
    func_80154960();
    if ((D_8015EDBC == 0) && (D_8015EDC0 == 0)) {
        D_8015EDC0 = 1;
    }
    if (D_8015EDC0 == 1) {
        D_8015EDC4 = func_80044C98();
        if (D_8015EDC4 == 1) {
            D_8015EDC0 = 2;
        }
    }
}

void func_8013840C(void) {
    if (D_8015EDBC == 0) {
        D_8015EDBC = 0x18;
        func_800450F4(1, 0x200);
        func_80042908(0xC);
    }
}
