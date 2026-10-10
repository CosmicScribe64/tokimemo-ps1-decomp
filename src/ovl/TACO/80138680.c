#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80138680", func_80138680);

void func_8013873C(void) {
    D_800E62B6 = 8;
    D_800E62B7 = 0x10;
    D_800E62B8 = 0x60;
    D_8015EDB0 = 4;
    func_8015ABF0();
    func_80132220();
    func_801334BC();
    D_8015EDBC = 0x34;
    D_8015EDC0 = 0;
    func_800450F4(0, 0x200);
    func_8004284C();
}

void func_801387C0(void) {
    func_80159090();
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

void func_80138840(void) {
    if (D_8015EDBC == 0) {
        D_8015EDBC = 0x38;
        func_8015ABF0();
        func_800450F4(1, 0x200);
        func_80042908(0x20);
    }
}
