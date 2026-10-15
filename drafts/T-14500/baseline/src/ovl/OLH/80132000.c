#include "common.h"
#include "ovl/OLH.h"

void func_80132000(void) {
    switch (D_800E6280.unk_1109) {
    case 0x0:
        func_80132114();
        break;
    case 0x1:
        func_80132294();
        break;
    case 0x2:
        func_801323AC();
        break;
    case 0x10:
        func_80132420();
        break;
    case 0x20:
        func_80132FA0();
        break;
    case 0x30:
        func_801364E0();
        break;
    case 0x40:
        func_80133B80();
        break;
    case 0x50:
        func_80134500();
        break;
    case 0x60:
        func_80134F40();
        break;
    case 0x70:
        func_80135AC0();
        break;
    }
    func_8006BA40();
    if (D_800E6280.unk_F88 & 0x800) {
        main_state_set(0x34);
    }
    func_801323CC();
    func_80066334();
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132000", func_80132114);

void func_80132294(void) {
    func_8004F984(0, D_8011ECF6, D_8011ECFA);
    func_80050B54(0);
    func_8004FC10(0);
    if ((D_800E6280.unk_F88 & 0x20) && D_800E6280.unk_1093 != -1) {
        func_80044750(0x501);
        switch (*(u8 *)&D_800E6280.unk_1093) { /* FAKE: u8 view keeps the selector load as lbu, as in the original (T-5010) */
        case 0:
            func_80042908(0x10);
            break;
        case 1:
            func_80042908(0x20);
            break;
        case 2:
            func_80042908(0x30);
            break;
        case 3:
            func_80042908(0x40);
            break;
        case 4:
            func_80042908(0x50);
            break;
        case 5:
            func_80042908(0x60);
            break;
        case 6:
            func_80042908(0x70);
            break;
        case 7:
            func_80042908(2);
            break;
        }
    }
}

void func_801323AC(void) {
    main_state_set(0x34);
}

void func_801323CC(void) {
    func_80049A40(-0x90, -0x58, 0x120, 0xC4, 4, 0xA0A0A0, 2);
    dtd_on(4);
}
