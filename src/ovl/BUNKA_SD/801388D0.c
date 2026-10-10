#include "common.h"
#include "ovl/BUNKA_SD.h"

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_801388D0);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_801389DC);

void func_80138ADC(void) {
    if ((u32)D_800E738A >= 3 && (u32)D_800E738A < 6 && (D_800E7200 & 0x40)) {
        func_80042940(5);
        func_80044750(0xC4);
    }
    func_80138B3C();
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_80138B3C);

void func_80138C34(void) {
    if (func_800460EC() & 4) {
        if (D_800E7380++ >= 1U) {
            func_80138D48();
            func_8004284C();
            func_80045414(9, 0, 0);
        }
    } else {
        D_800E7380 = 0;
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_80138CAC);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_80138D48);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_80139100);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_80139290);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_8013987C);
