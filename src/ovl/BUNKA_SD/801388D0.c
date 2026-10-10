#include "common.h"
#include "ovl/BUNKA_SD.h"

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_801388D0);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_801389DC);

void func_80138ADC(void) {
    if ((u32)D_800E6280.unk_110A >= 3 && (u32)D_800E6280.unk_110A < 6 && (D_800E6280.unk_F80 & 0x40)) {
        func_80042940(5);
        func_80044750(0xC4);
    }
    func_80138B3C();
}

void func_80138B3C(void) {
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80134260();
        func_80044750(0x7F);
        return;
    case 1:
        func_801389DC();
        return;
    case 2:
        func_80138C34();
        return;
    case 3:
        func_80134418();
        return;
    case 4:
        func_80138CAC();
        return;
    case 5:
        func_8004E9F4(1);
        func_8004284C();
        return;
    case 6:
        func_8013987C();
        return;
    case 7:
        func_801388D0();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80138C34(void) {
    if (func_800460EC() & 4) {
        if (D_800E6280.unk_1100++ >= 1U) {
            func_80138D48();
            func_8004284C();
            func_80045414(9, 0, 0);
        }
    } else {
        D_800E6280.unk_1100 = 0;
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_80138CAC);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_80138D48);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_80139100);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_80139290);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_8013987C);
