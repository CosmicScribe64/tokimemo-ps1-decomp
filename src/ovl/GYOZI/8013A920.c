#include "common.h"
#include "ovl/GYOZI.h"

void func_8013A920(void) {
    switch (D_801474B8) {
    case 0:
        func_8013A998();
        return;
    case 1:
        func_8013AAAC();
        return;
    case 2:
        func_8013AB34();
        return;
    default:
        func_8013ABBC();
        return;
    }
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A920", func_8013A998);

void func_8013AA20(void) {
    func_80086AB0(0x500);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A920", func_8013AA48);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A920", func_8013AAAC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A920", func_8013AB34);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A920", func_8013ABBC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A920", func_8013AC38);

void func_8013AD30(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void func_8013AD74(void) {
    D_801474A8 = 3;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void func_8013ADBC(void) {
    D_801474A8 = 6;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void func_8013AE04(void) {
    D_801474A8 = 9;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void func_8013AE4C(void) {
    func_8008A0D4(0x3FED);
    func_8004DE1C();
}
