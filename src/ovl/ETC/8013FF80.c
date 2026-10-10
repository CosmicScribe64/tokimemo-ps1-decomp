#include "common.h"
#include "ovl/ETC.h"

void func_8013FF80(void) {
    if (D_800E738A == 0) {
        func_8013FFAC();
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013FF80", func_8013FFAC);

void func_801407F0(void) {
    D_800E7380 += 1;
    switch (D_800E7389) {
    case 0:
        func_8013FF80();
        break;
    case 1:
        func_8013BF34();
        break;
    case 2:
        func_8013CF2C();
        break;
    case 0xFF:
        func_80140FD8();
        break;
    }
    func_80066334();
    if (D_800E7389 != 0xFF) {
        if (D_801500E0 == 0 && (D_800E7208 & 0x800)) {
            func_8013FC08();
            D_801500D4 = 0x800;
            func_8013FA7C();
            D_801500E0 = 1;
            func_80042908(2);
        }
    }
}
INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013FF80", func_801408F0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013FF80", func_80140AE4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013FF80", func_80140BC4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013FF80", func_80140DC8);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013FF80", func_80140E80);
