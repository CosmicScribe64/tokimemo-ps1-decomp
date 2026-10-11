#include "common.h"
#include "ovl/TACO.h"

void func_80138680(void) {
    if ((u32)D_800E6280.unk_1100 < 0x60 && (D_800E6280.unk_10F8 & 0x10)) {
        func_801432F0("3RD STAGE", -0x40, 0, 2);
    }
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013873C();
        break;
    case 1:
        func_801387C0();
        break;
    case 2:
        func_80138840();
        break;
    }
    func_8013F250();
    func_80132FE8();
    func_80133374();
}

void func_8013873C(void) {
    D_800E6280.unk_036 = 8;
    D_800E6280.unk_037 = 0x10;
    D_800E6280.unk_038 = 0x60;
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
