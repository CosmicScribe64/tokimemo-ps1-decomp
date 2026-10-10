#include "common.h"
#include "ovl/TACO.h"

void func_80138280(void) {
    if ((u32) D_800E6280.unk_1100 < 0x60 && (D_800E6280.unk_10F8 & 0x10)) {
        func_801432F0("1ST STAGE", -0x40, 0, 2);
    }
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80138348();
        break;
    case 1:
        func_8013838C();
        break;
    case 2:
        func_8013840C();
        break;
    }
    func_8013A790(1, 1);
    func_8013F250();
    func_80132FE8();
    func_80133374();
}

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
