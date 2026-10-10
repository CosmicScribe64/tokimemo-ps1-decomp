#include "common.h"
#include "ovl/GYOZI.h"

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801388E0", func_801388E0);

void func_80138954(void) {
    func_800504CC(1, 0xB290, 0xB271, 0xBB4E, 0xBB06, 0xBAF5);
    func_8004DE1C();
}

void func_80138998(void) {
    if (func_80050AB8() == 1) {
        func_80086AB0(0x201);
        func_8004DE1C();
    }
}

void func_801389D4(void) {
    func_80051DD8(7, 0x80197000, 0xA464);
    func_80138510();
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801388E0.rodata", D_801459E0);

void func_80138A10(void) {
    D_800F5AAE |= 0x10;
    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_800BCE10(&D_800D92A0, &D_801459E0);
    D_800F6412 = 0;
    D_800F6458 = 1;
    func_8009068C();
    D_800F53DA = 0x80;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    func_80138510();
    D_8014718C = D_80147100;
    D_80147190 = D_80147134;
    D_80147194 = D_80147168;
    D_80147198 = 0;
    D_8014719C = 0;
    func_8008F618(D_800F62CF = 0xC);
    D_800F5AAE |= 8;
    func_8004DE1C();
}
