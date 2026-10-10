#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80136B40", func_80136B40);

void func_80136C00(void) {
    func_8013AFDC();
    func_800450F4(0, 0x202);
    D_800E62B6 = 0;
    D_800E62B7 = 0;
    D_800E62B8 = 0;
    back_clear_switch(1);
    D_8015EDB0 = 0;
    func_80059048();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80136B40", func_80136C60);

void func_80136CD8(void) {
    func_80044750(0xC1);
    func_80042908(2);
}

void func_80136D00(void) {
    if (D_8015F4F4 < 0x100) {
        D_8015F4F0 = D_8015F4F4 / 2;
    }
    func_8013A790(1, 0);
    D_8015F4F4 += 4;
    if (D_8015F4F4 > 0x100) {
        D_8015F4F0 = 0x80;
        func_8004284C();
    }
}

void func_80136D78(void) {
    if (D_800E7200 & 0x40) {
        D_8015F400 += 0x78;
    } else {
        D_8015F400 += 0x3C;
    }
    func_8013A790(1, 0);
    func_80136FBC(D_8015F400, 3);
    if (D_8015F400 > 0x1000) {
        func_8004284C();
    }
}

void func_80136E08(void) {
    D_8015F400 += 0x1F4;
    func_8013A790(1, 0);
    func_80136FBC(D_8015F400, 0);
    if (D_8015F400 >= 0x9001) {
        D_8015F400 = 0;
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80136B40", func_80136E70);

void func_80136F4C(void) {
    func_8013A790(1, 0);
    func_80136FBC(0x1000, 3);
    if (D_800E7200 & 0x40) {
        func_8004284C();
    }
    if (D_800E7208 & 0x20) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80136B40", func_80136FBC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80136B40", func_801371B4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80136B40", func_80137264);
