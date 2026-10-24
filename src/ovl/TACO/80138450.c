#include "common.h"
#include "ovl/TACO.h"

void func_80138450(void) {
    if ((u32)D_800E6280.stage_timer < 0x60 && (D_800E6280.frame_count & 0x10)) {
        func_801432F0((s32)"2ND STAGE", -0x40, 0, 2);
    }
    D_800E6280.step_timer.w += 1;
    switch (D_800E6280.step) {
    case 0:
        func_80138520();
        break;
    case 1:
        func_801385B4();
        break;
    case 2:
        func_80138634();
        break;
    }
    func_8013F250();
    func_80132FE8();
    func_80133374();
}

void func_80138520(void) {
    func_800450F4(0, 0x203);
    func_80044750(0x24);
    func_8014394C();
    func_80132220();
    func_801334BC();
    D_800E6280.clear_color_r = 0x78;
    D_800E6280.clear_color_g = 0xA8;
    D_800E6280.clear_color_b = 0xF0;
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
