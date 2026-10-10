#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80138450", func_80138450);

void func_80138520(void) {
    func_800450F4(0, 0x203);
    func_80044750(0x24);
    func_8014394C();
    func_80132220();
    func_801334BC();
    D_800E62B6 = 0x78;
    D_800E62B7 = 0xA8;
    D_800E62B8 = 0xF0;
    D_8015EDB0 = 3;
    D_8015EDBC = 0x24;
    D_8015EDC0 = 0;
    func_8015ABF0();
    func_8004284C();
}

void func_801385B4(void) {
    func_80156A80();
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

void func_80138634(void) {
    if (D_8015EDBC == 0) {
        func_8015ABF0();
        D_8015EDBC = 0x28;
        func_800450F4(1, 0x201);
        func_80042908(0x16);
    }
}
