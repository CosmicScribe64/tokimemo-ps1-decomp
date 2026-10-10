#include "common.h"
#include "ovl/KANGEI.h"

void func_80132E40(void) {
    D_80139A50 = 0x801C1400;
    D_80139A54 = 0x801C1D6C;
    D_80139A58 = 0x801CA47C;
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_80132E74);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_80132FF8);

void func_801335A0(void) {
    D_800E699C += 1;
    D_80139AC4 = 0;
    D_80139AC8 = 0;
    func_80072338();
    hizuke_init();
    func_80042808();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_801335F0);

void func_80133734(void) {
    D_800B3D60 = 0;
    func_80044890(1, 0xC06F, 0xC065, 0xCC33, 0xCBE3, 0xCBD9);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_80044750(0x22);
    func_80044750(0x21);
    func_80044750(0x2F);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_801337B4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_80133834);

void func_801338B4(void) {
    if (D_800E7384 == 0) {
        func_8004500C(0, 0);
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_801338EC);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_80133A14);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_80133A54);
